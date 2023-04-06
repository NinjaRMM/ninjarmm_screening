/**
 * @file UtilsTest.cpp
 *
 * @brief Unit test for Utils templates.
 *
 * @author   gustavo.garcia
 * @date     Apr 02, 2023
 * @revision 1
 *
 */

/* INCLUDES ******************************************************************/

#include <gtest/gtest.h>
#include "Utils.hpp"

/* FIXTURE *******************************************************************/

class IsInBoundsTest : public testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

class ContainsTheStringTest : public testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

/* TEST CASES ****************************************************************/

TEST_F(IsInBoundsTest, InBoundTest) {
    // Test valid inputs
    EXPECT_TRUE(IsInBounds<int>(0, -1, 1));
    EXPECT_TRUE(IsInBounds<int>(1, 1, 10));
    EXPECT_TRUE(IsInBounds<int>(10, 0, 10));
}

TEST_F(IsInBoundsTest, OutBoundTest) {
    // Test invalid inputs
    EXPECT_FALSE(IsInBounds<int>(11, 5, 6));
    EXPECT_FALSE(IsInBounds<int>(-2, -1, 6));
    EXPECT_FALSE(IsInBounds<int>(11, 0, 10));
}

TEST_F(ContainsTheStringTest, OneTime) {
    std::vector<std::string> strings{"one", "two", "test"};
    auto p = [](const std::string& str) { return str == "test"; };
    int count = ContainsTheString(p, strings);
    EXPECT_EQ(count, 1);
}

TEST_F(ContainsTheStringTest, TwoTimes) {
    std::vector<std::string> strings{"book", "one", "two", "testing"};
    auto p = [](const std::string& str) { return str.size() == 3; };
    int count = ContainsTheString(p, strings);
    EXPECT_EQ(count, 2);
}

TEST_F(ContainsTheStringTest, NoneTime) {
    std::vector<std::string> strings{"one", "two", "five", "six"};
    auto p = [](const std::string& str) { return str.empty(); };
    int count = ContainsTheString(p, strings);
    EXPECT_EQ(count, 0);
}
