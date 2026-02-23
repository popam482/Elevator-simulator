#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "Building.h"
#include "Scheduler.h"
#include "FileLogger.h"
#include "ConsoleLogger.h"

using namespace std;

void createLogsDirectory() {
	system("if not exist logs mkdir logs");
}

int main(int argc, char* argv[]) {
	string inputFile = "input.txt";
	
	if (argc > 1) {
		inputFile = argv[1];
	}

	ifstream file(inputFile);
	if (!file.is_open()) {
		cerr << "Error: File " << inputFile << " cannot be found!" << endl;
		return 1;
	}

	createLogsDirectory();

	int floors = 0;
	int elevators = 0;
	vector<Passenger*> passengers;
	string line;
	bool readingPassengers = false;

	while (getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}

		if (line.find("FLOORS=") == 0) {
			floors = stoi(line.substr(7));
		}
		else if (line.find("ELEVATORS=") == 0) {
			elevators = stoi(line.substr(10));
		}
		else if (line.find("PASSENGERS") == 0) {
			readingPassengers = true;
			continue;
		}
		else if (readingPassengers && line.find("id=") == 0) {
			int id = 0, start = 0, dest = 0, arrival = 0;
			
			size_t pos = 0;
			
			if ((pos = line.find("id=")) != string::npos) {
				id = stoi(line.substr(pos + 3));
			}

			if ((pos = line.find("start=")) != string::npos) {
				start = stoi(line.substr(pos + 6));
			}

			if ((pos = line.find("dest=")) != string::npos) {
				dest = stoi(line.substr(pos + 5));
			}

			if ((pos = line.find("arrival=")) != string::npos) {
				arrival = stoi(line.substr(pos + 8));
			}

			if (start > 0 && dest > 0 && start != dest) {
				
				Passenger* p = new Passenger(start, dest, arrival);
				passengers.push_back(p);
			}
		}
	}
	file.close();

	if (floors == 0 || elevators == 0 || passengers.empty()) {
		cerr << "Error: Invalid configuration in input!" << endl;
		return 1;
	}

	cout << "=== ELEVATOR SIMULATOR ===" << endl;
	cout << "Floors: " << floors << endl;
	cout << "Elevators: " << elevators << endl;
	cout << "Passengers: " << passengers.size() << endl;
	cout << "=========================" << endl << endl;

	Building* building = new Building(floors, elevators);

	cout << "Choose output mode:" << endl;
	cout << "1 for file (logs/simulation.log)" << endl;
	cout << "2 for console" << endl;
	cout << "Choice: ";

	int choice;
	do {
		cin >> choice;
	} while (choice != 1 && choice != 2);

	Logger* chosenLogger;

	if (choice == 1)
		chosenLogger = new FileLogger("logs/simulation.log");
	else
		chosenLogger = new ConsoleLogger();

	Scheduler scheduler(building, passengers, chosenLogger);
	
	int maxTime = 500; 
	for (size_t i = 0; i < passengers.size(); i++) {
		Passenger* p = passengers[i];
		int estimatedTime = p->getArrivalTime() + abs(p->getDestinationFloor() - p->getStartFloor()) * 2 + 100;
		if (estimatedTime > maxTime) {
			maxTime = estimatedTime;
		}
	}

	cout << "Running simulation (max time: " << maxTime << " units)" << endl;
	scheduler.runSimulation(maxTime);

	scheduler.printStats();

	if (choice == 1)
		cout << "Simulation complete! Check logs/simulation.log for details." << endl;
	else
		cout << "Simulation complete! Check the console for details."<<endl;

	for (size_t i = 0; i < passengers.size(); i++) {
		delete passengers[i];
	}
	delete building;
	delete chosenLogger;

	return 0;
}
