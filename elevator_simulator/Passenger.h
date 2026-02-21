#pragma once
class Passenger
{
private:

	static int nextId;
	int passengerId;
	int startFloor;
	int destinationFloor;
	int arrivalTime;
	bool inElevator;

public:
	Passenger(int startFloor, int destinationFloor, int arrivalTime) {
		this->passengerId = nextId++;
		this->startFloor = startFloor;
		this->destinationFloor = destinationFloor;
		this->arrivalTime = arrivalTime;
		this->inElevator = false;
	}

	void setInElevator(bool inElev) {
		inElevator = inElev;
	}

	bool isDestinationFloor(int floor) {
		if (floor == destinationFloor)
			return true;
		return false;
	}

	int getId() {
		return passengerId;
	}

	int getStartFloor() {
		return startFloor;
	}

	int getArrivalTime() {
		return arrivalTime;
	}

	int getDestinationFloor() {
		return destinationFloor;
	}

	bool isInElevator() {
		return inElevator;
	}
};

