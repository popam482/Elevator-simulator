#include "Scheduler.h"
#include "Building.h"
#include <climits>
#include <set>
#include <algorithm>
using namespace std;

Scheduler::Scheduler(Building* b, vector<Passenger*>& passengers, Logger* l) {
    building = b;
    allPassengers = passengers;
    logger = l;
    currentTime = 0;
}

void Scheduler::runSimulation(int maxTime) {
    for (currentTime = 0; currentTime <= maxTime; currentTime++) {
        tick();
        if (allServed()) {
            logger->log("[Time: " + std::to_string(currentTime)
                + "] All passengers served. Simulation ending early.");
            break;
        }
    }
}

bool Scheduler::allServed() {
    for (auto& [floor, lst] : building->getWaitingPassengers())
        if (!lst.empty()) return false;
    for (auto& e : building->getElevators())
        if (e.getPassengerCount() > 0) return false;
    for (auto p : allPassengers)
        if (p->getArrivalTime() > currentTime) return false;
    return true;
}

void Scheduler::tick() {
    for (auto p : allPassengers)
        if (p->getArrivalTime() == currentTime)
            building->addWaitingPassenger(p);

    assignElevator();

    moveElevator();

    unboardPassengers();

    boardPassengers();

    building->logState(currentTime, logger);
}

void Scheduler::assignElevator() {
    auto& waitingMap = building->getWaitingPassengers();

    for (auto& [floor, q] : waitingMap) {
        if (q.empty()) continue;

        bool alreadyCovered = false;
        for (auto& el : building->getElevators()) {
            if (el.getStops().count(floor)) {
                alreadyCovered = true;
                break;
            }
        }
        if (alreadyCovered) continue;

        int passengerDir = (q.front()->getDestinationFloor() > floor) ? 1 : -1;

        Elevator* best = nullptr;
        int minCost = INT_MAX;

        for (auto& el : building->getElevators()) {
            if (el.getPassengerCount() >= el.getCapacity()) continue;

            int cost = INT_MAX;
            int state = el.getMovingState();
            int pos = el.getCurrentFloor();

            if (state == 0) {
                cost = abs(pos - floor);
            }
            else if (state == passengerDir) {
                bool notPassed = (state == 1 && pos <= floor) ||
                    (state == -1 && pos >= floor);
                if (notPassed) {
                    cost = abs(pos - floor); 
                }
            }

            if (cost < minCost) {
                minCost = cost;
                best = &el;
            }
        }

        if (best == nullptr) {
            for (auto& el : building->getElevators()) {
                if (el.getMovingState() != 0) continue;
                if (el.getPassengerCount() > 0) continue;
                int cost = abs(el.getCurrentFloor() - floor);
                if (cost < minCost) {
                    minCost = cost;
                    best = &el;
                }
            }
        }

        if (best != nullptr && best->getCurrentFloor() != floor) {
            best->addStop(floor);
        }
    }
}

void Scheduler::moveElevator() {
    for (auto& el : building->getElevators())
        if (el.getMovingState() != 0)
            el.move();
}

void Scheduler::boardPassengers() {
    for (auto& e : building->getElevators()) {
        auto& waiting = building->getWaitingPassengers()[e.getCurrentFloor()];
        if (waiting.empty()) continue;

        int elevDir = e.getMovingState();

        for (auto it = waiting.begin(); it != waiting.end(); ) {
            if (e.getPassengerCount() >= e.getCapacity()) break;

            Passenger* p = *it;
            int pDir = (p->getDestinationFloor() > e.getCurrentFloor()) ? 1 : -1;

            bool compatible = (elevDir == 0) || (pDir == elevDir);
            if (!compatible) {
                ++it;
                continue; 
            }

            if (!e.board(p)) break; 

            it = waiting.erase(it); 
            p->setBoardTime(currentTime);
            e.addStop(p->getDestinationFloor());

            if (elevDir == 0) {
                elevDir = pDir;
            }

            logger->log("[Time: " + to_string(currentTime) + "] Passenger "
                + to_string(p->getId()) + " boarded elevator " + to_string(e.getId()));
        }
    }
}
void Scheduler::unboardPassengers() {
    for (auto& e : building->getElevators()) {
        vector<Passenger*> exited = e.unboard();
        for (auto p : exited) {
            p->setDropOffTime(currentTime);
            logger->log("[Time: " + to_string(currentTime) + "] Passenger "
                + to_string(p->getId()) + " unboarded at floor "
                + to_string(e.getCurrentFloor()));
        }
    }
}

void Scheduler::printStats() {
    int totalWait = 0, maxWait = 0, served = 0, totalRide=0;
    for (auto p : allPassengers) {
        if (p->getDropOffTime() == -1) continue;
        int wait = p->getDropOffTime() - p->getArrivalTime();
        totalWait += wait;
        maxWait = std::max(maxWait, wait);
        if (p->getBoardTime() != -1) {
            int ride = p->getDropOffTime() - p->getBoardTime();
            totalRide += ride;
        }
        served++;
    }
    int unserved = (int)allPassengers.size() - served;

    int totalDistance = 0;
    for (auto& e : building->getElevators())
        totalDistance += e.getTotalFloorsTraveled();


    logger->log("\n=== SIMULATION COMPLETE ===");
    logger->log("Passengers served: " + std::to_string(served));
    if (unserved > 0)
        logger->log("Passengers unserved (not reached in time): "
            + std::to_string(unserved));
    if (served > 0) {
        logger->log("Average wait time: "
            + std::to_string(totalWait / served) + " ticks");
        logger->log("Maximum wait time: "
            + std::to_string(maxWait) + " ticks");
        logger->log("Average ride time: "
            + std::to_string(totalRide / served) + " ticks");
    }

    std::string utilLine = "Elevator utilization: ";
    bool first = true;
    for (auto& e : building->getElevators()) {
        int util = (currentTime > 0)
            ? (e.getBusyTicks() * 100 / currentTime)
            : 0;
        if (!first) utilLine += " ";
        utilLine += "E" + std::to_string(e.getId()) +
            "=" + std::to_string(util) + "%";
        first = false;
    }

    logger->log("Total distance traveled: " +
        std::to_string(totalDistance) + " floors");

    logger->log(utilLine);
    logger->log("Total ticks: " + std::to_string(currentTime));
}