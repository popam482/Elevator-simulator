#include "Scheduler.h"
#include "Building.h"
#include <fstream>
#include <climits>
#include <set>
using namespace std;

	Scheduler::Scheduler(Building* b, vector<Passenger*>& passengers, Logger *l) {
		building = b;
		allPassengers = passengers;
		logger = l;
	}

	void Scheduler::runSimulation(int maxTime) {
		for (currentTime = 0; currentTime <= maxTime; currentTime++) {
			tick();
		}
	}

	void Scheduler::tick() {

		for (auto p : allPassengers) {
			if (p->getArrivalTime() == currentTime)
				building->addWaitingPassenger(p);
		}

		assignElevator();
		moveElevator();

		boardPassengers();
		unboardPassengers();

		building->logState(currentTime, logger);
	}

	void Scheduler::assignElevator() {
		auto& waitingMap = building->getWaitingPassengers();
		std::set<int> assignedIds;

		for (auto it = waitingMap.begin(); it != waitingMap.end(); ++it) {
			int floor = it->first;
			auto& passengers = it->second;

			if (passengers.empty()) continue;

			Elevator* best = nullptr;
			int minDist = INT_MAX;

			for (auto& el : building->getElevators()) {
				if (assignedIds.count(el.getId())) continue;
				if (el.getPassengerCount() > 0) continue;      

				int dist = abs(el.getCurrentFloor() - floor);
				if (dist < minDist) {
					minDist = dist;
					best = &el;
				}
			}

			if (best != nullptr && best->getCurrentFloor() != floor) {
				best->setTargetFloor(floor);
				best->setMovingState(best->getCurrentFloor() < floor ? 1 : -1);
				assignedIds.insert(best->getId());
			}
		}
	}

	void Scheduler::moveElevator() {
		for (auto& el : building->getElevators()) {
			if (el.getMovingState() != 0) {
				el.move();
			}
			else {
				el.setMovingState(0);
			}
		}
	}

	void Scheduler::boardPassengers() {
		for (auto& e : building->getElevators()) {
			auto& waiting = building->getWaitingPassengers()[e.getCurrentFloor()];
			while (!waiting.empty()) {
				Passenger* p = waiting.front();
				bool hasBoarded=e.board(p);
				if (!hasBoarded)
					break;
				waiting.pop();
				p->setBoardTime(currentTime);

				int dest = p->getDestinationFloor();
				e.setTargetFloor(dest);
				if (e.getCurrentFloor() < dest)
					e.setMovingState(1);
				else
					e.setMovingState(-1);

				logger->log("[Time: " + to_string(currentTime) + "] Passenger " + to_string(p->getId())
					+ " boarded elevator " + to_string(e.getId()));
			}
		}
	}

	void Scheduler::unboardPassengers() {
		for (auto& e : building->getElevators()) {
			vector<Passenger*> exited = e.unboard();
			for (auto p : exited) {
				p->setDropOffTime(currentTime);
				logger->log("[Time: " + to_string(currentTime) + "] Passenger " + to_string(p->getId())
					+ " unboarded at floor " + to_string(e.getCurrentFloor()));
			}
		}
	}

	void Scheduler::printStats() {
		int totalWait = 0, maxWait = 0, served=0;
		for (auto p : allPassengers) {
			if (p->getDropOffTime() == -1)
				continue;
			int wait = p->getDropOffTime() - p->getArrivalTime();
			totalWait += wait;
			maxWait = std::max(maxWait, wait);
			served++;
		}
		logger->log("=== SIMULATION COMPLETE ===");
		logger->log("Passengers served: " + std::to_string(served));
		logger->log("Passengers unserved: " + std::to_string(allPassengers.size()-served));
		logger->log("Average wait time: " + std::to_string(totalWait / (int)allPassengers.size()) + " ticks");
		logger->log("Maximum wait time: " + std::to_string(maxWait) + " ticks");
		logger->log("Total ticks: " + std::to_string(currentTime));
	}


