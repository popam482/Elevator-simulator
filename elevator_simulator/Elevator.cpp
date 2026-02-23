#include "Elevator.h"

#include "Passenger.h"
#include <queue>
#include <vector>

using namespace std;

	Elevator::Elevator(int id, int currentFloor, int targetFloor) {
		this->id = id;
		this->currentFloor = currentFloor;
		this->targetFloor = targetFloor;
		movingState = 0;
	}

	void Elevator::move() {
		if (movingState == 1 && currentFloor < targetFloor)
			currentFloor++;
		else
			if (movingState == -1 && currentFloor > targetFloor)
				currentFloor--;
		if (currentFloor == targetFloor)
			movingState = 0;
	}

	bool Elevator::board(Passenger* p) {
		if ((int)passengers.size() >= MAX_CAPACITY)
			return false;
		else {
			passengers.push_back(p);
			p->setInElevator(true);
			return true;
		}
	}

	vector<Passenger*> Elevator::unboard() {
		vector<Passenger*> exited;
		auto it = passengers.begin();

		while (it != passengers.end()) {
			if ((*it)->getDestinationFloor() == currentFloor) {
				(*it)->setInElevator(false);
				exited.push_back(*it);
				it = passengers.erase(it);
			}
			else {
				it++;
			}
		}
		return exited;
	}

	bool Elevator::hasRequest() {
		return movingState != 0 || !passengers.empty();
	}

	int Elevator::getId() {
		return id;
	}

	int Elevator::getCurrentFloor() {
		return currentFloor;
	}

	void Elevator::setMovingState(int state) {
		movingState = state;
	}

	int Elevator::getMovingState() {
		return movingState;
	}

	int Elevator::getPassengerCount() {
		return passengers.size();
	}

	void Elevator::setTargetFloor(int floor) {
		targetFloor = floor;
	}

	int Elevator::getTargetFloor(){
		return targetFloor;
	}

