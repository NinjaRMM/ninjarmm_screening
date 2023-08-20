#include <gtest/gtest.h>

// Ideally this would not be a local include, a target would be configured and added 
// as a dependency for this test.
#include "../src/Jobs.hpp" 

TEST(JobsTest, AJustCreatedProgrammerDoesNotNeedAVacation) {

  Programmer programmer(true);
  EXPECT_FALSE(programmer.needVacation());
}

TEST(JobsTest, AJustCreatedPilotDoesNotNeedAVacation) {

  Pilot pilot;
  EXPECT_FALSE(pilot.needVacation());
}

TEST(JobsTest, APilotNeedsVacationAfterThreeStressfullDecisions) {

  Pilot pilot;
  for (size_t i = 0, iEnd = 3; i < iEnd; ++i) {
    pilot.makeDecision(true);
  }

  EXPECT_TRUE(pilot.needVacation());
}

TEST(JobsTest, AProgrammerNeedsVacationAfterTwentyOneStressfullDecisions) {

  Programmer programmer(false);
    for (size_t i = 0, iEnd = 21; i < iEnd; ++i) {
    programmer.makeDecision(true);
  }

  EXPECT_TRUE(programmer.needVacation());
}

TEST(JobsTest, AProgrammerWithStandingDoesNotGetStressed) {

  Programmer programmer(true);
  const uint stressBefore = programmer.currentStressLevel();
  programmer.makeDecision(true);
  const uint stressAfter = programmer.currentStressLevel();

  EXPECT_EQ(stressBefore, stressAfter);
}

TEST(JobsTest, AProgrammerWithStandingDeskCanHanddleAllTheStressInTheWorld) {

  Programmer programmer(true);
  const uint stressBefore = programmer.currentStressLevel();
  for (size_t i = 0, iEnd = 100; i < iEnd; ++i) {
    programmer.makeDecision(true);
  }
  const uint stressAfter = programmer.currentStressLevel();

  EXPECT_EQ(stressBefore, stressAfter);
  EXPECT_FALSE(programmer.needVacation());
}

TEST(JobsTest, APilotAlwaysMentionsParisWhenFlexing) {

  Pilot pilot;
  const std::string flexPhrase = pilot.flexOnParty(); 
  const size_t position = flexPhrase.find("Paris");

  EXPECT_NE(position, std::string::npos);
}

TEST(JobsTest, AProgrammerAlwaysMentionsArchWhenFlexing) {

  Programmer programmer(true);
  const std::string flexPhrase = programmer.flexOnParty(); 
  const size_t position = flexPhrase.find("Arch");

  EXPECT_NE(position, std::string::npos);
}
