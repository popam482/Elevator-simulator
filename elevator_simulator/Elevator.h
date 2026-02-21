#pragma once

#include "Passenger.h"
#include <queue>
#include <vector>

using namespace std;

class Elevator
{
private:

	int id;
	int currentFloor;
	vector<Passenger*> passengers;
	int movingState; // -1-moving down, 0-not moving, 1-moving up 
	int targetFloor;

public:
	Elevator(int id, int currentFloor, int targetFloor=1) {
		this->id = id;
		this->currentFloor = currentFloor;
		this->targetFloor = targetFloor;
		movingState = 0;
	}

	void move() {
		if (movingState == 1 && currentFloor < targetFloor)
			currentFloor++;
		else
			if (movingState == -1 && currentFloor > targetFloor)
				currentFloor--;
		if(currentFloor==targetFloor)
			movingState=0;
	}

	void board(Passenger* p) {
		passengers.push_back(p);
		p->setInElevator(true);
	}

	vector<Passenger*> unboard() {
		vector<Passenger*> exited;
		auto it=passengers.begin();

		while (it!=passengers.end()) {
			if((*it)->getDestinationFloor()==currentFloor) {
				(*it)->setInElevator(false);
				exited.push_back(*it);
				it=passengers.erase(it);
			} else {
				it++;
			}
		}
		return exited;
	}

	bool hasRequest() {
		return movingState!=0 || !passengers.empty();
	}

	int getId() {
		return id;
	}

	int getCurrentFloor() {
		return currentFloor;
	}

	void setMovingState(int state) {
		movingState = state;
	}

	int getMovingState() {
		return movingState;
	}

	int getPassengerCount() {
		return passengers.size();
	}

	void setTargetFloor(int floor) {
    	targetFloor = floor;
	}

	int getTargetFloor() const {
    	return targetFloor;
	}

};

