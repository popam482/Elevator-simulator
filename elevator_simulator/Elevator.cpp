#include "Elevator.h"
#include "Passenger.h"
#include <vector>
#include <set>
#include <climits>
#include <cstdlib>
using namespace std;

Elevator::Elevator(int id, int currentFloor, int targetFloor) {
    this->id = id;
    this->currentFloor = currentFloor;
    this->targetFloor = targetFloor;
    this->movingState = 0;
}

void Elevator::move() {
    if (movingState == 0) return;

    if (movingState == 1)
        currentFloor++;
    else if (movingState == -1)
        currentFloor--;

    if (stops.count(currentFloor)) {
        stops.erase(currentFloor);
        targetFloor = currentFloor;
    }

    if (stops.empty()) {
        movingState = 0;
    }
    else {
        updateDirection();
    }
}

bool Elevator::board(Passenger* p) {
    if ((int)passengers.size() >= MAX_CAPACITY)
        return false;
    passengers.push_back(p);
    p->setInElevator(true);
    return true;
}

vector<Passenger*> Elevator::unboard() {
    vector<Passenger*> exited;
    auto it = passengers.begin();
    while (it != passengers.end()) {
        if ((*it)->getDestinationFloor() == currentFloor) {
            (*it)->setInElevator(false);
            exited.push_back(*it);
            it = passengers.erase(it);
        }
        else {
            it++;
        }
    }
    return exited;
}

bool Elevator::hasRequest() {
    return movingState != 0 || !stops.empty() || !passengers.empty();
}

int Elevator::getId() { return id; }
int Elevator::getCurrentFloor() { return currentFloor; }
void Elevator::setMovingState(int state) { movingState = state; }
int Elevator::getMovingState() { return movingState; }
int Elevator::getPassengerCount() { return (int)passengers.size(); }

void Elevator::addStop(int floor) {
    if (floor == currentFloor) return; 
    stops.insert(floor);
    updateDirection();
}

void Elevator::removeStop(int floor) {
    stops.erase(floor);
}

bool Elevator::hasStops() const {
    return !stops.empty();
}

int Elevator::getNextStop() const {
    if (stops.empty()) return currentFloor;

    if (movingState == 1) {
        for (int s : stops) {
            if (s > currentFloor) return s;
        }
        return *stops.rbegin();
    }
    else if (movingState == -1) {
        for (auto it = stops.rbegin(); it != stops.rend(); ++it) {
            if (*it < currentFloor) return *it;
        }
        return *stops.begin();
    }
    else {
        int best = *stops.begin();
        int bestDist = abs(best - currentFloor);
        for (int s : stops) {
            int d = abs(s - currentFloor);
            if (d < bestDist) {
                bestDist = d;
                best = s;
            }
        }
        return best;
    }
}

void Elevator::updateDirection() {
    if (stops.empty()) {
        movingState = 0;
        return;
    }
    int next = getNextStop();
    if (next > currentFloor) movingState = 1;
    else if (next < currentFloor) movingState = -1;
    else movingState = 0;
}

void Elevator::setTargetFloor(int floor) {
    targetFloor = floor;
    addStop(floor);
}

int Elevator::getTargetFloor() { return targetFloor; }

const set<int>& Elevator::getStops() const {
    return stops;
}