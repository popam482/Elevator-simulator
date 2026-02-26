# Elevator Control System Simulator

This project implements an optimized algorithm to manage multiple elevators in a multi-story building.

## Features
- Multiple elevators with nearest-car dispatching
- Data-driven configuration via `input.txt`
- Dual output: file log or console
- OOP design: Building, Elevator, Passenger, Scheduler, Logger hierarchy
- Unit testing (with Google Test) to ensure the reliability of the core simulation logic
- Statistics (including Average Wait Time, Max Wait Time, and Total Simulation Ticks) at the end of the simulation

## Installation & Build
1. Clone the repository: git clone https://github.com/popam482/Elevator-simulator.git
2. Open `elevator_simulator.sln` in Visual Studio 2022
3. Set `elevator_simulator` as the Startup Project
4. Edit `input.txt` with your configuration
5. Press **Ctrl+F5** to run
6. Choose output mode:
   - `1` : logs written to `logs/simulation.log`
   - `2` : logs printed to console

## Usage
Default input file: `input.txt`

## Input Format
```
FLOORS=10
ELEVATORS=3
CAPACITY=5
MAX_TIME=1000

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

## Unit testing
Tests are written with **Google Test**, included natively in Visual Studio 2022 (no manual installation required).

### Test project setup

The `ElevatorTests` project lives in the same solution as the main simulator.

| Test                        | Scenario                                              |
|-----------------------------|-------------------------------------------------------|
| `SinglePassengerSingleElevator` | 1 passenger, 1 elevator, direct trip            |
| `TwoPassengersSameDirection`    | 2 passengers going up, shared elevator          |
| `PassengerGoingDown`            | Passenger travelling downward                   |
| `AllPassengersServed`           | Mixed traffic, 3 elevators, all must be served  |
| `LateArrival`                   | Passenger arriving at tick 50, not before       |
| `CapacityRespected`             | 6 passengers, capacity 2 — multiple trips       |


### Running the tests
1. Build the solution: **Ctrl+Shift+B**
2. Open **Test → Test Explorer**
3. Click **Run All** in the Test Explorer toolbar
