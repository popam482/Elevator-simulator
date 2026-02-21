#pragma once
#include "Building.h"
#include "Logger.h"
#include <fstream>
#include <climits>

class Scheduler
{
private:

	Building* building;
	std::vector<Passenger*> allPassengers;
	int currentTime;
	Logger* logger;

public:

	Scheduler(Building* b, std::vector<Passenger*>& passengers, Logger *l);

	void runSimulation(int maxTime);

	void tick();

	void assignElevator();

	void moveElevator();

	void boardPassengers();

	void unboardPassengers();

};

