#include "step1/Pilot.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(PilotTest, Getters) {
    step1::Pilot p;
    EXPECT_EQ(p.GetName(), "Pilot");
    EXPECT_EQ(p.GetDescription(), "racing a car");
    EXPECT_EQ(p.GetRequiredHours(), 12);
}
