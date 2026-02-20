#include "Passenger.h"
#include <queue>
using namespace std;

class Elevator
{
private:

	int id;
	int currentFloor;
	queue<Passenger*> passengers;
	int movingState; // -1-moving down, 0-not moving, 1-moving up 

public:
	Elevator(int id, int currentFloor) {
		this->id = id;
		this->currentFloor = currentFloor;
		movingState = 0;
	}

	void move() {
		if (movingState == 1)
			currentFloor++;
		else
			if (movingState == -1)
				currentFloor--;
	}

	void board(Passenger* p) {
		passengers.push(p);
		p->isInElevator(true);
	}

	vector<Passenger*> unboard() {
		std::vector<Passenger*> exited;
		size_t n = passengers.size();

		std::queue<Passenger*> temp;

		while (!passengers.empty()) {
			Passenger* p = passengers.front();
			passengers.pop();

			if (p->isDestinationFloor(currentFloor)) {
				exited.push_back(p);
				p->isInElevator(false);
			}
			else {
				temp.push(p);
			}
		}

		passengers = temp;

		return exited;
	}

	bool hasRequest() {
		return !passengers.empty();
	}

	int getId() {
		return id;
	}

	int getCurrentFloor() {
		return currentFloor;
	}

};

