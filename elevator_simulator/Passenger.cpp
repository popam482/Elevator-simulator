#include "Passenger.h"

	int Passenger::nextId = 1;

	Passenger::Passenger(int startFloor, int destinationFloor, int arrivalTime) {
		this->passengerId = nextId++;
		this->startFloor = startFloor;
		this->destinationFloor = destinationFloor;
		this->arrivalTime = arrivalTime;
		this->inElevator = false;
	}

	void Passenger::setInElevator(bool inElev) {
		inElevator = inElev;
	}

	bool Passenger::isDestinationFloor(int floor) {
		if (floor == destinationFloor)
			return true;
		return false;
	}

	int Passenger::getId() {
		return passengerId;
	}

	int Passenger::getStartFloor() {
		return startFloor;
	}

	int Passenger::getArrivalTime() {
		return arrivalTime;
	}

	int Passenger::getDestinationFloor() {
		return destinationFloor;
	}

	bool Passenger::isInElevator() {
		return inElevator;
	}