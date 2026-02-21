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
	Passenger(int startFloor, int destinationFloor, int arrivalTime);

	void setInElevator(bool inElev);

	bool isDestinationFloor(int floor);

	int getId();

	int getStartFloor();

	int getArrivalTime();

	int getDestinationFloor();

	bool isInElevator();
};

