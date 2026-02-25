# Elevator Control System Simulator

This project implements an optimized algorithm to manage multiple elevators in a multi-story building.

## Features
- Multiple elevators with nearest-car dispatching
- Data-driven configuration via `input.txt`
- Dual output: file log or console
- OOP design: Building, Elevator, Passenger, Scheduler, Logger hierarchy
- Statistics (including Average Wait Time, Max Wait Time, and Total Simulation Ticks) at the end of the simulation

## Installation & Build
- Clone the repository: git clone https://github.com/popam482/Elevator-simulator.git
- Open `elevator_simulator.sln` in Visual Studio 2022 and build.
C++ version: ISO C++ 20 Standard

## Usage
Default input file: `input.txt`

## Input Format
```
FLOORS=10
ELEVATORS=3
PASSENGERS
id=1 start=1 dest=5 arrival=0
id=2 start=3 dest=8 arrival=2
id=3 start=5 dest=3 arrival=1
```

## Architecture
```
Logger (abstract)
├── FileLogger
└── ConsoleLogger

Building
├── vector<Elevator>
└── map<floor, list<Passenger*>>

Elevator
├──vector<Passenger*> passengers
└──set<int> stops

```

Scheduler: It orchestrates the tick() cycle, managing the sequence of events: adding new arrivals, processing unboarding/boarding, and invoking the dispatching logic.

Elevator: It maintains its own internal state (current floor, direction, and passenger load) and manages an optimized std::set of destination stops.

Passenger: Each passenger object tracks its own lifecycle—from arrival time to boarding and drop-off.

Building : It manages the number of floors and holds the floor-specific lists where Passenger entities wait for an available elevator.

Logger Interface: A polymorphic logging system, allowing for extensible reporting to consoles or persistent files.
