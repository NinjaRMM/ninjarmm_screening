#include <gtest/gtest.h>
#include <Step1.hpp>

class JobTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        programmer_ptr_ = std::make_unique<Programmer>();
        pilot_ptr_ = std::make_unique<Pilot>();
    }

    void TearDown() override {
        programmer_ptr_.reset();
        pilot_ptr_.reset();
    }

    std::unique_ptr<Programmer> programmer_ptr_;
    std::unique_ptr<Pilot> pilot_ptr_;
};

TEST_F(JobTestFixture, TestProgrammer) {
    EXPECT_EQ(programmer_ptr_->get_name(), "Programmer");
    EXPECT_EQ(programmer_ptr_->get_description(), "writing code");
    EXPECT_EQ(programmer_ptr_->get_hours_required(), 40);
    EXPECT_NO_THROW(programmer_ptr_->DoWork());
}

TEST_F(JobTestFixture, TestPilot) {
    EXPECT_EQ(pilot_ptr_->get_name(), "Pilot");
    EXPECT_EQ(pilot_ptr_->get_description(), "flying a plane");
    EXPECT_EQ(pilot_ptr_->get_hours_required(), 28);
    EXPECT_NO_THROW(pilot_ptr_->DoWork());
}

// ********** ********** **********

class IsInBoundsTestFixture : public ::testing::TestWithParam<std::tuple<uint32_t, uint32_t, uint32_t, bool>> 
{
    ; // empty body
};

TEST_P(IsInBoundsTestFixture, TestIsInBounds) {
    auto [evaluated_value, lower_bound, upper_bound, expected] = GetParam();
    EXPECT_EQ(is_in_bounds(evaluated_value, lower_bound, upper_bound), expected);
}

INSTANTIATE_TEST_CASE_P(TestValues, IsInBoundsTestFixture,
    testing::Values(
        std::make_tuple(0, 0, 25, true),
        std::make_tuple(5, 1, 8, true),
        std::make_tuple(10, 0, 10, true),
        std::make_tuple(11, 0, 10, false),
        std::make_tuple(5, 5, 10, true),
        std::make_tuple(1, 5, 10, false),
        std::make_tuple(4, 5, 10, false),
        std::make_tuple(11, 5, 10, false)
    )
);

// ********** ********** **********

class CountMatchesParametrizedTestFixture 
    : public testing::TestWithParam<std::tuple<std::vector<std::string>, std::function<bool(const std::string&)>, int>>
{
    ; // empty body
};

TEST_P(CountMatchesParametrizedTestFixture, TestCountMatches)
{
    auto [operable_items, callback, expected_count] = GetParam();
    int count = count_matches(operable_items, callback);
    EXPECT_EQ(count, expected_count);
}

INSTANTIATE_TEST_CASE_P(CountMatchesParametrizedTest, CountMatchesParametrizedTestFixture,
                        testing::Values(
                            std::make_tuple(std::vector<std::string>{"heello!!!!", "world", "program"}, [](const std::string& str) { return str.size() > 5; }, 2),
                            std::make_tuple(std::vector<std::string>{"night", "for", "fun"}, [](const std::string& str) { return str == "fun"; }, 1),
                            std::make_tuple(std::vector<std::string>{"Underground", "passionate", "is"}, [](const std::string& str) { return str.size() < 5; }, 1)));