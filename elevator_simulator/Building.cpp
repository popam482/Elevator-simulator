#include "Building.h"
#include "Elevator.h"
#include <vector> 
#include <map>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

	Building::Building(int floors, int elevatorsNum, int initialTargetFloor) {
		this->floors = floors;
		for (int i = 0; i < elevatorsNum; i++) {
			elevators.emplace_back(i + 1, 1, initialTargetFloor);
		}
	}

	void Building::addWaitingPassenger(Passenger* p) {
		waitingPassengers[p->getStartFloor()].push(p);
	}

	void Building::logState(int currentTime, Logger *logger) {
		
		stringstream ss;

		ss << "[" << setfill('0') << setw(2) << currentTime << "]";

		bool hadMovement = false;
		for (auto& e : elevators) {
			if (e.getMovingState() != 0) {
				if (hadMovement) ss<<" | ";
				string direction = (e.getMovingState() == 1) ? "up" : "down";
				ss<<"Elevator " << e.getId() << " moving " << direction << " (floor " << e.getCurrentFloor() << ")";
				hadMovement = true;
			}
		}

		if (!hadMovement) {
			ss<<"All elevators idle";
		}
			logger->log(ss.str());
	}

	vector<Elevator>& Building::getElevators() {
		return elevators;
	}

	int Building::getWaitingPassengersNum() {
		int total = 0;
		for (const auto& [floor, q] : waitingPassengers)
			total += q.size();
		return total;
	}

	map<int, queue<Passenger*>>& Building::getWaitingPassengers() {
		return waitingPassengers;
	}


