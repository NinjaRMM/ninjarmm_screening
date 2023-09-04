#include <gtest/gtest.h>
#include "Job.h"
#include "Pilot.h"
#include "Programmer.h"
#include "functions.h"


TEST(Inheritance, Pilot)
{
    auto pilot = Pilot{"Pilot for ACME Airlines", 10000U};
    
    EXPECT_EQ("Pilot", pilot.GetName());
    EXPECT_EQ("Pilot for ACME Airlines", pilot.GetDescription());
    EXPECT_EQ(44U, pilot.GetWorkHours());
    EXPECT_EQ(10000U, pilot.GetFlightTime());
    EXPECT_EQ("The pilot must have at least 10000 hours of flight time.", pilot.MinimumRequirements());
}

TEST(Inheritance, Programmer)
{
    auto programmer = Programmer{"C++ Programmer", "C++, CMake, GTest"};
    
    EXPECT_EQ("Programmer", programmer.GetName());
    EXPECT_EQ("C++ Programmer", programmer.GetDescription());
    EXPECT_EQ(40U, programmer.GetWorkHours());
    EXPECT_EQ("C++, CMake, GTest", programmer.GetStack());
    EXPECT_EQ("The programmer must have at least: C++, CMake, GTest", programmer.MinimumRequirements());
}

TEST(Bounds, IsInBounds)
{
    EXPECT_FALSE(IsInBounds(0, 1, 5));
    EXPECT_TRUE(IsInBounds(1, 1, 5));
    EXPECT_TRUE(IsInBounds(2, 1, 5));
    EXPECT_TRUE(IsInBounds(5, 1, 5));
    EXPECT_FALSE(IsInBounds(6, 1, 5));
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
