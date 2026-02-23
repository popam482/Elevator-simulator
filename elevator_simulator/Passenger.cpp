#include "Passenger.h"

	int Passenger::nextId = 1;

	Passenger::Passenger(int startFloor, int destinationFloor, int arrivalTime) {
		this->passengerId = nextId++;
		this->startFloor = startFloor;
		this->destinationFloor = destinationFloor;
		this->arrivalTime = arrivalTime;
		this->inElevator = false;

		this->waitStartTime = arrivalTime;
		this->boardTime = -1;
		this->dropoffTime = -1;
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

	void Passenger::setBoardTime(int time) {
		boardTime = time;
	}

	void Passenger::setDropOffTime(int time) {
		dropoffTime = time;
	}

	int Passenger::getBoardTime() {
		return boardTime;
	}

	int Passenger::getDropOffTime() {
		return dropoffTime;
	}