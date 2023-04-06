/**
 * @file ProgrammerTest.cpp
 *
 * @brief Unit test for programmer class.
 *
 * @author   gustavo.garcia
 * @date     Apr 02, 2023
 * @revision 1
 *
 */

/* INCLUDES ******************************************************************/

#include <gtest/gtest.h>
#include <iostream>
#include "Programmer.hpp"

/* FIXTURE *******************************************************************/

class ProgrammerTest : public ::testing::Test {
protected:
    Programmer programmer;
};

/* TEST CASES ****************************************************************/

TEST_F(ProgrammerTest, GetName) {
    std::string name = programmer.GetName();

    EXPECT_EQ(name, "Programmer");
}

TEST_F(ProgrammerTest, GetHoursRequired) {
    int hours = programmer.GetHoursRequired();

    EXPECT_EQ(hours, 41);
}

TEST_F(ProgrammerTest, GetDescription) {
    std::string desc = programmer.GetDescription();

    EXPECT_EQ(desc, "Writing good quality code.");
}


TEST_F(ProgrammerTest, DoWork) {
    testing::internal::CaptureStdout();

    programmer.DoWork();

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "My work involves Writing good quality code.\n");
}
