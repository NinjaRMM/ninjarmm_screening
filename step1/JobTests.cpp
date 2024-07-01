
#include "pch.h"
#include "Job.hpp"
#include <gtest/gtest.h>

class JobTests: public testing::Test
{
public:
	Job m_job1{};
};

TEST_F(JobTests, Action1)
{
	EXPECT_EQ(m_job1.Action1(), 0);
}

TEST_F(JobTests, Action2)
{
	EXPECT_EQ(m_job1.Action2(), false);
}

TEST_F(JobTests, Action3)
{
	EXPECT_EQ(m_job1.Action3(), "");
}

TEST_F(JobTests, Action4)
{
	EXPECT_EQ(m_job1.Action4(), 0.0f);
}