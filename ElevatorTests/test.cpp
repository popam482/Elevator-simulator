#include <gtest/gtest.h>
#include "Elevator.h"
#include "Passenger.h"
#include "Building.h"
#include "Scheduler.h"
#include "ConsoleLogger.h"
#include "Logger.h"
#include <vector>
#include <sstream>

//Passenger tests

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

//Elevator tests

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


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}