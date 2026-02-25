#pragma once
#include <vector>
#include <set>
#include "Passenger.h"

class Elevator {
public:
    static const int MAX_CAPACITY = 10;

    Elevator(int id, int currentFloor, int targetFloor);

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
    bool hasStops() const;
    int getNextStop() const;      
    void updateDirection();       
    const std::set<int>& getStops() const;

    void setTargetFloor(int floor);
    int getTargetFloor();

private:
    int id;
    int currentFloor;
    int targetFloor;
    int movingState;

    std::vector<Passenger*> passengers;
    std::set<int> stops;
};