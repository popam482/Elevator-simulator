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
	int waitStartTime = -1; 
	int boardTime = -1;      
	int dropoffTime = -1;

public:
	Passenger(int startFloor, int destinationFloor, int arrivalTime);

	void setInElevator(bool inElev);

	bool isDestinationFloor(int floor);

	int getId();

	int getStartFloor();

	int getArrivalTime();

	int getDestinationFloor();

	bool isInElevator();

	void setBoardTime(int time);

	void setDropOffTime(int time);

	int getBoardTime();
};

