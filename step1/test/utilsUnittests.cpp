#include "gtest/gtest.h"

#include "../src/Utils/Utils.h"

using namespace std;
using namespace ninjarmm_screening_step1;

TEST(Utils, IsInBounds) 
{
    EXPECT_FALSE(IsInBounds<uint32_t>(1, 2, 10));
    EXPECT_FALSE(IsInBounds<double>(1.5, 2, 10));
    EXPECT_FALSE(IsInBounds<double>(1.99, 2, 10));
    EXPECT_TRUE(IsInBounds<double>(2, 2, 10));
    EXPECT_TRUE(IsInBounds<double>(10, 2, 10));
}

TEST(Utils, ContainsTheString) 
{
    auto theStrings = vector<string> { "one", "two", "test"};
    EXPECT_EQ(ContainsTheString([](const string& tested) { return tested == "test"; }, theStrings), 1);
    EXPECT_EQ(ContainsTheString([](const string& tested) { return tested == "no_test"; }, theStrings), 0);
}

TEST(Utils, SumVariadicArguments) 
{
    EXPECT_EQ(SumVariadicArguments(1,2,3,4),10);
    EXPECT_EQ(SumVariadicArguments(string("Hello"),string(","),string(" I am a ninja")),"Hello, I am a ninja");
}