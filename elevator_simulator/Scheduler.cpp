#include "Scheduler.h"
#include "Building.h"
#include <fstream>
#include <climits>
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
		for (auto it = waitingMap.begin(); it != waitingMap.end(); ++it) {
			int floor = it->first;
			auto& passengers = it->second;

			if (passengers.empty()) continue;

			Elevator* nearest = nullptr;
			int minDist = INT_MAX;

			for (auto& el : building->getElevators()) {
				int dist = abs(el.getCurrentFloor() - floor);
				if (dist < minDist) {
					minDist = dist;
					nearest = &el;
				}
			}

			if (nearest != nullptr && nearest->getCurrentFloor() != floor) {
				nearest->setTargetFloor(floor);
				if (nearest->getCurrentFloor() < floor) {
					nearest->setMovingState(1);
				}
				else {
					nearest->setMovingState(-1);
				}

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
				waiting.pop();
				e.board(p);
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
		int totalWait = 0, maxWait = 0;
		for (auto p : allPassengers) {
			int wait = p->getBoardTime() - p->getArrivalTime();
			totalWait += wait;
			maxWait = std::max(maxWait, wait);
		}
		logger->log("=== SIMULATION COMPLETE ===");
		logger->log("Passengers served: " + std::to_string(allPassengers.size()));
		logger->log("Average wait time: " + std::to_string(totalWait / (int)allPassengers.size()) + " ticks");
		logger->log("Maximum wait time: " + std::to_string(maxWait) + " ticks");
		logger->log("Total ticks: " + std::to_string(currentTime));
	}


