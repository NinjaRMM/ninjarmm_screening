/**
 * @file test_step1.cpp
 * @author Guilherme El Kadri Ribeiro (guilhermeelkadri@gmail.com)
 * @brief Test material NinjaRMM - Step 1
 * @version 0.1
 * @date 2023-06-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <gtest/gtest.h>
#include "step1.hpp"

// Google Test: Test IsInBounds function
TEST(IsInBoundsTest, CheckRange) 
{
    EXPECT_TRUE(IsInBounds<uint32_t>(503, 500, 599));
    EXPECT_FALSE(IsInBounds<uint32_t>(400, 500, 599));
    EXPECT_FALSE(IsInBounds<uint32_t>(600, 500, 599));
}

// Google Test: Test ContainsTheString function
TEST(ContainsTheStringTest, CheckCount) 
{
    auto theStrings = std::vector<std::string> { "one", "two", "test" };
    int count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    EXPECT_EQ(count, 1);
}

int main() 
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}