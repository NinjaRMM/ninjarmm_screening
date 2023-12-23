
#include <gtest/gtest.h>

#include <iostream>

#include "Pilot.h"
#include "Programmer.h"

using namespace std;
using namespace ninjans;

/**
 *  Unit tests using google test for step1.
 *  Need to install google test on the machine. (https://github.com/google/googletest/blob/main/googletest/README.md)
 */

TEST(JobTest, PilotTest)
{
	Pilot pilot;
	EXPECT_EQ(pilot.Title(), "Co-pilot");
	EXPECT_EQ(pilot.IsRemoteWorker(), false);
	EXPECT_EQ(pilot.HasUniform(), true);
	EXPECT_EQ(pilot.Work(), "I fly planes.");
}

TEST(JobTest, ProgrammerTest)
{
	Programmer programmer;
	EXPECT_EQ(programmer.Title(), "Software Engineer");
	EXPECT_EQ(programmer.IsRemoteWorker(), true);
	EXPECT_EQ(programmer.HasUniform(), false);
	EXPECT_EQ(programmer.Work(), "I write computer programs.");
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();

}
