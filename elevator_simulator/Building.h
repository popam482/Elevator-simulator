#pragma once
#include "Elevator.h"
#include <vector> 
#include <map>
#include <fstream>

using namespace std;

class Building
{
private:
	int floors;
	vector<Elevator> elevators;
	map<int, queue<Passenger*>> waitingPassengers;

public:
	Building(int floors, int elevatorsNum) {
		this->floors = floors;
		for (int i = 0; i < elevatorsNum; i++)
			elevators.emplace_back(i + 1);
	}

	void addWaitingPassenger(Passenger* p) {
		waitingPassengers[p->getStartFloor()].push(p);
	}

	void logState(int currentTime, ofstream& logFile) {
		logFile << "[Time: " << currentTime << "]\n";
		for (auto& e : elevators) {
			logFile << "Elevator " << e.getId() << " at floor " << e.getCurrentFloor() << "\n";
		}
		logFile << "\n";
	}

	vector<Elevator>& getElevators() {
		return elevators;
	}

	int getWaitingPassengersNum() {
		return waitingPassengers.size();
	}

	map<int, queue<Passenger*>>& getWaitingPassengers() {
		return waitingPassengers;
	}

};

