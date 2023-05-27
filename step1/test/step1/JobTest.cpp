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

struct MockJob: public step1::Job<TestLogger> {
    MockJob(const std::string &name, unsigned requiredHours)
        : step1::Job<TestLogger>(name, requiredHours){};
    MOCK_METHOD(std::string, GetDescription, (), (const, override));
};

std::weak_ptr<TestLogger> TestLogger::instance;

template class step1::Job<TestLogger>;

struct JobTest: public testing::Test {
    const std::string name = "Driver";
    const std::string description = "drives around town";
    const unsigned int hours = 10;

    MockJob job;

    JobTest(): job(name, hours) {}
};

TEST_F(JobTest, Construction) {
    EXPECT_EQ(job.GetName(), name);
    EXPECT_EQ(job.GetRequiredHours(), hours);
}

TEST_F(JobTest, DoWork) {
    using testing::Return;

    auto logger = std::make_shared<TestLogger>();
    TestLogger::instance = logger;

    EXPECT_CALL(job, GetDescription).WillOnce(Return(description));

    EXPECT_CALL(*logger,
                LogImplementation("My work involves " + description + "\n"));
    job.DoWork();
}
