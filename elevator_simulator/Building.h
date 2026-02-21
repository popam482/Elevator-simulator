#pragma once
#include "Elevator.h"
#include <vector> 
#include <map>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

class Building
{
private:
	int floors;
	vector<Elevator> elevators;
	map<int, queue<Passenger*>> waitingPassengers;

public:
	Building(int floors, int elevatorsNum, int initialTargetFloor = 1) {
		this->floors = floors;
		for (int i = 0; i < elevatorsNum; i++) {
			elevators.emplace_back(i + 1, 1, initialTargetFloor);
		}
	}

	void addWaitingPassenger(Passenger* p) {
		waitingPassengers[p->getStartFloor()].push(p);
	}

	void logState(int currentTime, ofstream& logFile) {
		logFile << "[" << setfill('0') << setw(2) << currentTime << "] ";
		
		bool hadMovement = false;
		for (auto& e : elevators) {
			if (e.getMovingState() != 0) {
				if (hadMovement) logFile << " | ";
				string direction = (e.getMovingState() == 1) ? "up" : "down";
				logFile << "Elevator " << e.getId() << " moving " << direction << " (floor " << e.getCurrentFloor() << ")";
				hadMovement = true;
			}
		}
		
		if (!hadMovement) {
			logFile << "All elevators idle";
		}
		logFile << "\n";
	}

	vector<Elevator>& getElevators() {
		return elevators;
	}

	int getWaitingPassengersNum() {
		int total = 0;
    	for (const auto& [floor, q] : waitingPassengers)
        	total += q.size();
    	return total;
	}

	map<int, queue<Passenger*>>& getWaitingPassengers() {
		return waitingPassengers;
	}

};

