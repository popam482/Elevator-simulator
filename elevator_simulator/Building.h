#pragma once
#include "Elevator.h"
#include "Logger.h"
#include <vector>
#include <map>
#include <iomanip>
#include <string>
#include <list>

class Building {
private:
    int floors;
    std::vector<Elevator> elevators;
    std::map<int, std::list<Passenger*>> waitingPassengers; 

public:
    Building(int floors, int elevatorsNum, int maxCap, int initialTargetFloor = 1);

    void addWaitingPassenger(Passenger* p);
    void logState(int currentTime, Logger* logger);
    std::vector<Elevator>& getElevators();
    int getWaitingPassengersNum();
    std::map<int, std::list<Passenger*>>& getWaitingPassengers();
    bool hasFinished();
};