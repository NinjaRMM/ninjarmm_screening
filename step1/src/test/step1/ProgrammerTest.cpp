#include "step1/Programmer.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(ProgrammerTest, Getters) {
    step1::Programmer p;
    EXPECT_EQ(p.GetName(), "Programmer");
    EXPECT_EQ(p.GetDescription(), "writing code");
    EXPECT_EQ(p.GetRequiredHours(), 8);
}
