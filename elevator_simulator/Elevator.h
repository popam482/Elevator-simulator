#pragma once
#include <vector>
#include <set>
#include "Passenger.h"

class Elevator {

private:
    int id;
    int currentFloor;
    int targetFloor;
    int movingState;
    int MAX_CAPACITY;
    int totalFloorsTraveled;
    int busyTicks;

    std::vector<Passenger*> passengers;
    std::set<int> stops;

public:

    Elevator(int id, int currentFloor, int targetFloor, int maxCap);

    void move();
    bool board(Passenger* p);
    std::vector<Passenger*> unboard();
    bool hasRequest();

    int getId();
    int getCurrentFloor();
    void setMovingState(int state);
    int getMovingState();
    int getPassengerCount();

    void addStop(int floor);
    void removeStop(int floor);
    bool hasStops();
    int getNextStop();
    void updateDirection();
    std::set<int>& getStops();

    void setTargetFloor(int floor);
    int getTargetFloor();

    int getCapacity();
    int getTotalFloorsTraveled();
    int getBusyTicks();

};