#include <gtest/gtest.h>
#include "Elevator.h"
#include "Passenger.h"
#include "Building.h"
#include "Scheduler.h"
#include "ConsoleLogger.h"
#include "Logger.h"
#include <vector>
#include <sstream>


TEST(PassengerTest, ConstructorSetsFields) {
    Passenger p(3, 10, 5);
    EXPECT_EQ(p.getStartFloor(), 3);
    EXPECT_EQ(p.getDestinationFloor(), 10);
    EXPECT_EQ(p.getArrivalTime(), 5);
    EXPECT_EQ(p.getBoardTime(), -1);
    EXPECT_EQ(p.getDropOffTime(), -1);
    EXPECT_FALSE(p.isInElevator());
}

TEST(PassengerTest, BoardAndDropOffTimes) {
    Passenger p(1, 5, 0);
    p.setBoardTime(3);
    p.setDropOffTime(7);
    EXPECT_EQ(p.getBoardTime(), 3);
    EXPECT_EQ(p.getDropOffTime(), 7);
}

TEST(PassengerTest, IsDestinationFloor) {
    Passenger p(1, 8, 0);
    EXPECT_TRUE(p.isDestinationFloor(8));
    EXPECT_FALSE(p.isDestinationFloor(5));
}

TEST(PassengerTest, InElevatorFlag) {
    Passenger p(2, 9, 0);
    EXPECT_FALSE(p.isInElevator());
    p.setInElevator(true);
    EXPECT_TRUE(p.isInElevator());
    p.setInElevator(false);
    EXPECT_FALSE(p.isInElevator());
}


TEST(ElevatorTest, InitialState) {
    Elevator e(1, 1, 1, 5);
    EXPECT_EQ(e.getId(), 1);
    EXPECT_EQ(e.getCurrentFloor(), 1);
    EXPECT_EQ(e.getMovingState(), 0);   
    EXPECT_EQ(e.getPassengerCount(), 0);
    EXPECT_FALSE(e.hasStops());
}

TEST(ElevatorTest, AddStopSetsDirection) {
    Elevator e(1, 1, 1, 5);
    e.addStop(5);
    EXPECT_TRUE(e.hasStops());
    EXPECT_EQ(e.getMovingState(), 1);   
}

TEST(ElevatorTest, AddStopDown) {
    Elevator e(1, 10, 10, 5);
    e.addStop(3);
    EXPECT_EQ(e.getMovingState(), -1);
}

TEST(ElevatorTest, MoveUp) {
    Elevator e(1, 1, 1, 5);
    e.addStop(3);
    e.move(); 
    EXPECT_EQ(e.getCurrentFloor(), 2);
    e.move();
    EXPECT_EQ(e.getCurrentFloor(), 3);
    EXPECT_EQ(e.getMovingState(), 0);   
    EXPECT_FALSE(e.hasStops());
}

TEST(ElevatorTest, MoveDown) {
    Elevator e(1, 5, 5, 5);
    e.addStop(2);
    e.move(); e.move(); e.move();
    EXPECT_EQ(e.getCurrentFloor(), 2);
    EXPECT_EQ(e.getMovingState(), 0);
}

TEST(ElevatorTest, BoardPassenger) {
    Elevator e(1, 1, 1, 5);
    Passenger p(1, 5, 0);
    EXPECT_TRUE(e.board(&p));
    EXPECT_EQ(e.getPassengerCount(), 1);
    EXPECT_TRUE(p.isInElevator());
}

TEST(ElevatorTest, BoardRespectsCapacity) {
    Elevator e(1, 1, 1, 2);  
    Passenger p1(1, 5, 0), p2(1, 6, 0), p3(1, 7, 0);
    EXPECT_TRUE(e.board(&p1));
    EXPECT_TRUE(e.board(&p2));
    EXPECT_FALSE(e.board(&p3)); 
    EXPECT_EQ(e.getPassengerCount(), 2);
}

TEST(ElevatorTest, UnboardCorrectFloor) {
    Elevator e(1, 5, 5, 5);
    Passenger p(1, 5, 0);
    e.board(&p);
    auto exited = e.unboard();
    EXPECT_EQ(exited.size(), 1u);
    EXPECT_EQ(exited[0]->getId(), p.getId());
    EXPECT_EQ(e.getPassengerCount(), 0);
    EXPECT_FALSE(p.isInElevator());
}

TEST(ElevatorTest, UnboardWrongFloor) {
    Elevator e(1, 3, 3, 5);
    Passenger p(1, 5, 0);
    e.board(&p);
    auto exited = e.unboard(); 
    EXPECT_TRUE(exited.empty());
    EXPECT_EQ(e.getPassengerCount(), 1);
}

TEST(ElevatorTest, MultipleStopsInOrder) {
    Elevator e(1, 1, 1, 5);
    e.addStop(3);
    e.addStop(5);
    e.addStop(7);
    while (e.getMovingState() != 0 || e.hasStops()) {
        e.move();
    }
    EXPECT_EQ(e.getCurrentFloor(), 7);
}

TEST(ElevatorTest, TotalFloorsTraveled) {
    Elevator e(1, 1, 1, 5);
    e.addStop(4);
    e.move(); e.move(); e.move(); 
    EXPECT_EQ(e.getTotalFloorsTraveled(), 3);
}

TEST(ElevatorTest, BusyTicks) {
    Elevator e(1, 1, 1, 5);
    e.addStop(3);
    e.move();
    e.move(); 
    EXPECT_EQ(e.getBusyTicks(), 2);
}


TEST(BuildingTest, AddWaitingPassenger) {
    Building b(10, 2, 5);
    Passenger p(3, 8, 0);
    b.addWaitingPassenger(&p);
    EXPECT_EQ(b.getWaitingPassengersNum(), 1);
}

TEST(BuildingTest, HasFinishedWhenEmpty) {
    Building b(10, 2, 5);
    EXPECT_TRUE(b.hasFinished()); // niciun pasager
}

TEST(BuildingTest, HasNotFinishedWithWaiting) {
    Building b(10, 2, 5);
    Passenger p(1, 5, 0);
    b.addWaitingPassenger(&p);
    EXPECT_FALSE(b.hasFinished());
}

TEST(SimulationTest, SinglePassengerSingleElevator) {
    Building* b = new Building(10, 1, 5);
    std::vector<Passenger*> passengers;
    Passenger* p = new Passenger(1, 5, 0);
    passengers.push_back(p);

    ConsoleLogger logger;
    Scheduler s(b, passengers, &logger);
    s.runSimulation(100);

    EXPECT_EQ(p->getDropOffTime(), 4); 
    EXPECT_NE(p->getDropOffTime(), -1);

    delete p;
    delete b;
}


TEST(SimulationTest, TwoPassengersSameDirection) {
    Building* b = new Building(10, 1, 5);
    std::vector<Passenger*> passengers;
    Passenger* p1 = new Passenger(1, 3, 0);
    Passenger* p2 = new Passenger(1, 6, 0);
    passengers.push_back(p1);
    passengers.push_back(p2);

    ConsoleLogger logger;
    Scheduler s(b, passengers, &logger);
    s.runSimulation(100);

    EXPECT_NE(p1->getDropOffTime(), -1);
    EXPECT_NE(p2->getDropOffTime(), -1);
    EXPECT_LT(p1->getDropOffTime(), p2->getDropOffTime()); 

    delete p1; delete p2;
    delete b;
}


TEST(SimulationTest, PassengerGoingDown) {
    Building* b = new Building(10, 1, 5);
    std::vector<Passenger*> passengers;
    Passenger* p = new Passenger(8, 2, 0);
    passengers.push_back(p);

    ConsoleLogger logger;
    Scheduler s(b, passengers, &logger);
    s.runSimulation(200);

    EXPECT_NE(p->getDropOffTime(), -1);
    EXPECT_EQ(p->getDropOffTime() - p->getBoardTime(), 6);

    delete p;
    delete b;
}


TEST(SimulationTest, AllPassengersServed) {
    Building* b = new Building(20, 3, 5);
    std::vector<Passenger*> passengers;

    passengers.push_back(new Passenger(1, 10, 0));
    passengers.push_back(new Passenger(1, 15, 0));
    passengers.push_back(new Passenger(5, 2, 2));
    passengers.push_back(new Passenger(8, 18, 3));
    passengers.push_back(new Passenger(20, 1, 5));

    ConsoleLogger logger;
    Scheduler s(b, passengers, &logger);
    s.runSimulation(500);

    for (auto p : passengers) {
        EXPECT_NE(p->getDropOffTime(), -1)
            << "Passenger " << p->getId() << " was NOT served!";
    }

    for (auto p : passengers) delete p;
    delete b;
}

TEST(SimulationTest, LateArrival) {
    Building* b = new Building(10, 1, 5);
    std::vector<Passenger*> passengers;
    Passenger* p = new Passenger(1, 5, 50); 
    passengers.push_back(p);

    ConsoleLogger logger;
    Scheduler s(b, passengers, &logger);
    s.runSimulation(200);

    EXPECT_GE(p->getBoardTime(), 50);   
    EXPECT_NE(p->getDropOffTime(), -1);

    delete p;
    delete b;
}

TEST(SimulationTest, CapacityRespected) {
    Building* b = new Building(10, 1, 2); 
    std::vector<Passenger*> passengers;
    for (int i = 0; i < 6; i++)
        passengers.push_back(new Passenger(1, 5, 0));

    ConsoleLogger logger;
    Scheduler s(b, passengers, &logger);
    s.runSimulation(500);

    for (auto p : passengers)
        EXPECT_NE(p->getDropOffTime(), -1);

    for (auto p : passengers) delete p;
    delete b;
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}