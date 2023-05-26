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
    MOCK_METHOD(std::string, GetName, (),  (const, override));
    MOCK_METHOD(std::string, GetDescription, (),  (const, override));
    MOCK_METHOD(unsigned int, GetRequiredHours, (),  (const, override));
};

std::weak_ptr<TestLogger> TestLogger::instance;

template class step1::Job<TestLogger>;

struct JobTest: public testing::Test {
    const std::string name = "Driver";
    const std::string description = "drives around town";
    const unsigned int hours = 10;

    MockJob job;
};

TEST_F(JobTest, Getters) {
    // I decided to leave this test to showcase my GooGleMock and Test Fixture usage
    using testing::Return;
    EXPECT_CALL(job, GetName).Times(2).WillRepeatedly(Return(name));
    EXPECT_CALL(job, GetDescription).Times(2).WillRepeatedly(Return(description));
    EXPECT_CALL(job, GetRequiredHours).WillOnce(Return(hours));

    EXPECT_EQ(job.GetName(), name);
    EXPECT_EQ(job.GetDescription(), description);
    EXPECT_EQ(job.GetRequiredHours(), hours);

    EXPECT_NE(job.GetName(), job.GetDescription());
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
