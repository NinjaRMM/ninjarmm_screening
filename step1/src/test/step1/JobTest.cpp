#include "step1/Job.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

struct TestLogger {
    static std::weak_ptr<TestLogger> instance;

    MOCK_METHOD(void, LogImplementation, (const std::string&), ());
    static void Log(const std::string& message) {
        if (auto logger = instance.lock()) {
            logger->LogImplementation(message);
        }
    }
};

std::weak_ptr<TestLogger> TestLogger::instance;

template class step1::Job<TestLogger>;

struct JobTest: public testing::Test {
    const std::string name = "Driver";
    const std::string description = "drives around town";
    const unsigned int hours = 10;

    step1::Job<TestLogger> job;

    JobTest(): job(name, description, hours) {}
};

TEST_F(JobTest, Getters) {
    EXPECT_EQ(job.GetName(), name);
    EXPECT_EQ(job.GetDescription(), description);
    EXPECT_NE(job.GetName(), job.GetDescription());
    EXPECT_EQ(job.GetRequiredHours(), hours);
}

TEST_F(JobTest, DoWork) {
    auto logger = std::make_shared<TestLogger>();
    TestLogger::instance = logger;

    EXPECT_CALL(
        *logger,
        LogImplementation("My work involves " + description + "\n")
    );
    job.DoWork();
}
