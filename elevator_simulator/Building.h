#pragma once
#include "Elevator.h"
#include "Logger.h"
#include <vector> 
#include <map>
#include <fstream>
#include <iomanip>
#include <string>


class Building
{
private:
	int floors;
	std::vector<Elevator> elevators;
	std::map<int, std::queue<Passenger*>> waitingPassengers;

public:
	Building(int floors, int elevatorsNum, int initialTargetFloor = 1);

	void addWaitingPassenger(Passenger* p);

	void logState(int currentTime, Logger* logger);

	std::vector<Elevator>& getElevators();

	int getWaitingPassengersNum();

	std::map<int, std::queue<Passenger*>>& getWaitingPassengers();

	bool hasFinished();

};

