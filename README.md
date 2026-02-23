# Elevator Control System Simulator

A C++ simulation of a multi-elevator building system with configurable floors, elevators, and passengers.

## Features
- Multiple elevators with nearest-car dispatching
- Data-driven configuration via `input.txt`
- Dual output: file log or console
- OOP design: Building, Elevator, Passenger, Scheduler, Logger hierarchy

## Build
Open `elevator_simulator.sln` in Visual Studio 2022 and build.

## Usage
Default input file: `input.txt`

## Input Format
```
FLOORS=10
ELEVATORS=3
PASSENGERS
id=1 start=1 dest=5 arrival=0
id=2 start=3 dest=8 arrival=2
```

## Architecture
```
Logger (abstract)
├── FileLogger
└── ConsoleLogger

Building
├── vector<Elevator>
└── map<floor, queue<Passenger*>>

```

Scheduler : drives the simulation tick by tick
