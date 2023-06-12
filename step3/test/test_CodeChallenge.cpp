/**
 * @file test_CodeChallenge.cpp
 * @author Guilherme El Kadri Ribeiro (guilhermeelkadri@gmail.com)
 * @brief Test material NinjaRMM - Step 3
 * @version 0.1
 * @date 2023-06-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <gtest/gtest.h>
#include "CodeChallenge.hpp" 

// Define a test fixture
class CheckIfPositiveTest : public ::testing::Test 
{
protected:
    // Declare objects to be used in the tests
    OBJ_INT objInt;
    OBJ_STR objStr;
};

// Define test cases
TEST_F(CheckIfPositiveTest, IntResult) 
{
    int result = checkIfPositive<int, OBJ_INT>(objInt, 1, 2);
    EXPECT_EQ(result, 1);
}

TEST_F(CheckIfPositiveTest, IntResultWithNegativeParams) 
{
    int result = checkIfPositive<int, OBJ_INT>(objInt, 1, -2);
    EXPECT_EQ(result, 0);
}

TEST_F(CheckIfPositiveTest, IntResultWithOptionalParam) 
{
    int result = checkIfPositive<int, OBJ_INT>(objInt, 1, -2, 3);
    EXPECT_EQ(result, 1);
}

TEST_F(CheckIfPositiveTest, StringResult) 
{
    std::string result = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2);
    EXPECT_EQ(result, "true");
}

TEST_F(CheckIfPositiveTest, StringResultWithNegativeParams) 
{
    std::string result = checkIfPositive<std::string, OBJ_STR>(objStr, 1, -2);
    EXPECT_EQ(result, "false");
}

TEST_F(CheckIfPositiveTest, StringResultWithOptionalParam) 
{
    std::string result = checkIfPositive<std::string, OBJ_STR>(objStr, 1, -2, 3);
    EXPECT_EQ(result, "true");
}

TEST_F(CheckIfPositiveTest, IntResultWithTotalParam) 
{
    int total = 0;
    int result = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, 3, &total);
    EXPECT_EQ(result, 1);
    EXPECT_EQ(total, 6);
}

TEST_F(CheckIfPositiveTest, StringResultWithTotalParam) 
{
    int total = 0;
    std::string result = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, 3, &total);
    EXPECT_EQ(result, "true");
    EXPECT_EQ(total, 6);
}

TEST_F(CheckIfPositiveTest, IntResultWithNonPositiveResponse) 
{
    int result = checkIfPositive<int, OBJ_INT>(objInt, -1, -2);
    EXPECT_EQ(result, 0);
}

TEST_F(CheckIfPositiveTest, IntResultWithNonPositiveResponseAndOptionalParam) 
{
    int result = checkIfPositive<int, OBJ_INT>(objInt, -1, -2, -3);
    EXPECT_EQ(result, 0);
}

TEST_F(CheckIfPositiveTest, StringResultWithNonPositiveResponse) 
{
    std::string result = checkIfPositive<std::string, OBJ_STR>(objStr, -1, -2);
    EXPECT_EQ(result, "false");
}

TEST_F(CheckIfPositiveTest, StringResultWithNonPositiveResponseAndOptionalParam) 
{
    std::string result = checkIfPositive<std::string, OBJ_STR>(objStr, -1, -2, -3);
    EXPECT_EQ(result, "false");
}

// Run the tests
int main() 
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}