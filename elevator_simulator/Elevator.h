#pragma once

#include "Passenger.h"
#include <queue>
#include <vector>


class Elevator
{
private:

	int id;
	int currentFloor;
	std::vector<Passenger*> passengers;
	int movingState; // -1-moving down, 0-not moving, 1-moving up 
	int targetFloor;

public:
	Elevator(int id, int currentFloor, int targetFloor = 1);

	void move();

	void board(Passenger* p);

	std::vector<Passenger*> unboard();

	bool hasRequest();

	int getId();

	int getCurrentFloor();

	void setMovingState(int state);

	int getMovingState();

	int getPassengerCount();

	void setTargetFloor(int floor);

	int getTargetFloor();

};