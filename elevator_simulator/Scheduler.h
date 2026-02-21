#pragma once
#include "Building.h"
#include <fstream>
#include <climits>
using namespace std;

class Scheduler
{
private:

	Building* building;
	vector<Passenger*> allPassengers;
	int currentTime;
	ofstream logFile;

public:

	Scheduler(Building* b, vector<Passenger*>& passengers) {
		building = b;
		allPassengers = passengers;
		logFile.open("logs/simulation.log");
	}

	~Scheduler() {
		if (logFile.is_open())
			logFile.close();
	}

	void runSimulation(int maxTime) {
		for (currentTime = 0; currentTime <= maxTime; currentTime++) {
			tick();
		}
		logFile.close();
	}

	void tick() {
		
		for (auto p : allPassengers) {
			if (p->getArrivalTime() == currentTime)
				building->addWaitingPassenger(p);
		}

		assignElevator();
		moveElevator();
		
		boardPassengers();
		unboardPassengers();

		building->logState(currentTime, logFile);
	}

	void assignElevator() {

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
				} else {
					nearest->setMovingState(-1); 
				}
		
			}
		}
	}

	void moveElevator() {
		for (auto& el : building->getElevators()) {
			if (el.getMovingState() != 0) {
				el.move();
			} else {
				el.setMovingState(0); 
			}
		}
	}

	void boardPassengers() {
		for (auto& e : building->getElevators()) {
			auto& waiting = building->getWaitingPassengers()[e.getCurrentFloor()];
			while (!waiting.empty()) {
				Passenger* p = waiting.front();
				waiting.pop();
				e.board(p);

				int dest = p->getDestinationFloor();
            	e.setTargetFloor(dest);
            	if(e.getCurrentFloor() < dest)
                	e.setMovingState(1);
            	else
                	e.setMovingState(-1);

				logFile << "[Time: " << currentTime << "] Passenger " << p->getId()
					<< " boarded elevator " << e.getId() << "\n";
			}
		}
	}

	void unboardPassengers() {
		for (auto& e : building->getElevators()) {
			vector<Passenger*> exited = e.unboard();
			for (auto p : exited) {
				logFile << "[Time: " << currentTime << "] Passenger " << p->getId()
					<< " unboarded at floor " << e.getCurrentFloor() << "\n";
			}
		}
	}

};

