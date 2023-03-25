#include "gtest/gtest.h"
#include "Step1.hpp"

TEST(JobTest, Getters)
{
    Job job("Test Job", "Test Description", 5);

    EXPECT_EQ(job.GetName(), "Test Job");
    EXPECT_EQ(job.GetDescription(), "Test Description");
    EXPECT_EQ(job.GetHoursRequired(), 5);
}

TEST(JobTest, DoWork)
{
    Job job("Test Job", "Test Description", 5);

    testing::internal::CaptureStdout();
    job.DoWork();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "My work involves Test Description\n");
}

TEST(ProgrammerTest, DoWork)
{
    Programmer programmer;

    testing::internal::CaptureStdout();
    programmer.DoWork();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "My work involves Programming\n");
}

TEST(PilotTest, DoWork)
{
    Pilot pilot;

    testing::internal::CaptureStdout();
    pilot.DoWork();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "My work involves Flying\n");
}

TEST(ContainsTheStringTest, EmptyCollection)
{
    std::vector<std::string> strings;
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, strings);
    EXPECT_EQ(count, 0);
}

TEST(ContainsTheStringTest, NonEmptyCollectionNoMatches)
{
    std::vector<std::string> strings{"one", "two", "three"};
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, strings);
    EXPECT_EQ(count, 0);
}

TEST(ContainsTheStringTest, NonEmptyCollectionOneMatch)
{
    std::vector<std::string> strings{"one", "two", "test"};
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, strings);
    EXPECT_EQ(count, 1);
}

TEST(ContainsTheStringTest, NonEmptyCollectionMultipleMatches)
{
    std::vector<std::string> strings{"one", "test", "test"};
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, strings);
    EXPECT_EQ(count, 2);
}

TEST(IsInBoundsTest, InBounds)
{
    uint32_t value = 550;
    bool inBounds = IsInBounds<uint32_t>(value, 500, 599);
    EXPECT_TRUE(inBounds);
}

TEST(IsInBoundsTest, BelowLowerBound)
{
    uint32_t value = 499;
    bool inBounds = IsInBounds<uint32_t>(value, 500, 599);
    EXPECT_FALSE(inBounds);
}

TEST(IsInBoundsTest, AboveUpperBound)
{
    uint32_t value = 600;
    bool inBounds = IsInBounds<uint32_t>(value, 500, 599);
    EXPECT_FALSE(inBounds);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}