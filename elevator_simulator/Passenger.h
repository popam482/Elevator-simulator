#pragma once
class Passenger
{
private:

	static int id;
	int startFloor;
	int destinationFloor;
	int arrivalTime;
	int inElevator;

public:
	Passenger(int id, int startFloor, int destinationFloor, int arrivalTime) {
		this->id = id++;
		this->startFloor = startFloor;
		this->destinationFloor = destinationFloor;
		this->arrivalTime = arrivalTime;
	}

	void isInElevator(bool inElev) {
		inElevator = inElev;
	}

	bool isDestinationFloor(int floor) {
		if (floor == destinationFloor)
			return true;
		return false;
	}

	int getId() {
		return id;
	}

	int getStartFloor() {
		return startFloor;
	}

	int getArrivalTime() {
		return arrivalTime;
	}
};

int Passenger::id = 1;

