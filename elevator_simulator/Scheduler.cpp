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

    unboardPassengers();

    boardPassengers();

    assignElevator();

    moveElevator();

    building->logState(currentTime, logger);
}

void Scheduler::assignElevator() {
    auto& waitingMap = building->getWaitingPassengers();

    for (auto& [floor, lst] : waitingMap) {
        if (lst.empty()) continue;

        bool alreadyCovered = false;
        for (auto& el : building->getElevators()) {
            if (el.getStops().count(floor)) {
                alreadyCovered = true;
                break;
            }
            if (el.getMovingState() == 0 && el.getCurrentFloor() == floor) {
                alreadyCovered = true;
                break;
            }
        }
        if (alreadyCovered) continue;

        int passengerDir = (lst.front()->getDestinationFloor() > floor) ? 1 : -1;

        Elevator* best = nullptr;
        int minDist = INT_MAX;

        for (auto& el : building->getElevators()) {
            if (el.getPassengerCount() >= Elevator::MAX_CAPACITY) continue;
            int elDir = el.getMovingState();
            if (elDir == 0) continue;

            if (elDir == 1 && el.getCurrentFloor() >= floor) continue;
            if (elDir == -1 && el.getCurrentFloor() <= floor) continue;

            if (elDir != passengerDir) continue;

            int dist = abs(el.getCurrentFloor() - floor);
            if (dist < minDist) {
                minDist = dist;
                best = &el;
            }
        }

        if (best == nullptr) {
            minDist = INT_MAX;
            for (auto& el : building->getElevators()) {
                if (el.getMovingState() != 0) continue;
                if (el.getPassengerCount() > 0) continue;
                if (el.hasStops()) continue;

                int dist = abs(el.getCurrentFloor() - floor);
                if (dist < minDist) {
                    minDist = dist;
                    best = &el;
                }
            }
        }

        if (best == nullptr) continue;

        if (best->getCurrentFloor() != floor)
            best->addStop(floor);
    }
}

void Scheduler::moveElevator() {
    for (auto& el : building->getElevators())
        if (el.getMovingState() != 0)
            el.move();
}

void Scheduler::boardPassengers() {
    for (auto& e : building->getElevators()) {
        if (e.getMovingState() != 0) continue;

        auto& waiting = building->getWaitingPassengers()[e.getCurrentFloor()];
        if (waiting.empty()) continue;

        int elevDir = e.getMovingState();

        auto it = waiting.begin();
        while (it != waiting.end()) {
            Passenger* p = *it;
            if (e.getPassengerCount() > 0) {
                int nextStop = e.getNextStop();
                int liftDir = (nextStop > e.getCurrentFloor()) ? 1 : -1;
                int pDir = (p->getDestinationFloor() > e.getCurrentFloor()) ? 1 : -1;
                if (pDir != liftDir) {
                    ++it;
                    continue;
                }
            }

            bool hasBoarded = e.board(p);
            if (!hasBoarded) break;

            it = waiting.erase(it); 
            p->setBoardTime(currentTime);
            e.addStop(p->getDestinationFloor());

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
    int totalWait = 0, maxWait = 0, served = 0;
    for (auto p : allPassengers) {
        if (p->getDropOffTime() == -1) continue;
        int wait = p->getDropOffTime() - p->getArrivalTime();
        totalWait += wait;
        maxWait = std::max(maxWait, wait);
        served++;
    }
    int unserved = (int)allPassengers.size() - served;

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
    }
    logger->log("Total ticks: " + std::to_string(currentTime));
}