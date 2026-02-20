#pragma once
#include "Building.h"
#include <fstream>
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
	}

	void moveElevator() {

		for (auto& el : building->getElevators()) {
			if (el.hasRequest()) {
				el.move();
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

