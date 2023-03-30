//Unit Tests for step 1
#include "../Job.h"
#include "../step1_functions.h"
#include <string>
#include <iostream>
#include <gmock/gmock.h>
using namespace testing;
using namespace ninjaone_screening;
using namespace std::string_literals;
using std::string; using std::ostream;
using std::ostringstream;
using std::vector; using std::unique_ptr;

#ifdef WIN32
int main(int argc, wchar_t** argv)
#else 
int main(int argc, char** argv)
#endif
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

//All tests are placed in one file for ease of review.
static const string k_description_prefix = "My work involves "s;

class Mock_Printer : public Job::Printer {
public:
    virtual Mock_Printer& operator<<(const std::string& value)override{
        m_ostream << value;
        return *this;
    }
    ostringstream m_ostream;
};

TEST(Job_Tests, Programmer_Name) {
    Programmer programmer;
    /* For this and all the following expects we could instead of hard coding "Programmer" or 50 for hours 
    * refer to some global variable that the production code uses. ie expose sk_programmer_job_name from the 
    * programmer classes production code. however changes to that variable would then not be detected. By hard coding here
    * it becomes clear and documented what is expected so if sk_programmer_name variable is erroneously changed
    * it will be detected by the test. If we do in fact intend to change the name/behavior we must also update
    * the unit tests.
    */
    EXPECT_STREQ("Programmer", programmer.name().c_str());
}

TEST(Job_Tests, Programmer_Description) {
    Programmer programmer;
    EXPECT_STREQ("creating and maintaining code", programmer.description().c_str());
}

TEST(Job_Tests, Programmer_Hours) {
    Programmer programmer;
    EXPECT_EQ(50, programmer.hours());
}

TEST(Job_Tests, Programmer_WorkTest) {
    Programmer programmer;
    Mock_Printer* mock_printer = new Mock_Printer;
    programmer.set_printer(mock_printer);

    const string expected_output = k_description_prefix + programmer.description();

    programmer.DoWork();
    EXPECT_STREQ(expected_output.c_str(), mock_printer->m_ostream.str().c_str());
}

TEST(Job_Tests, Pilot_Name) {
    Pilot pilot;
    EXPECT_STREQ("Pilot", pilot.name().c_str());
}

TEST(Job_Tests, Pilot_Description) {
    Pilot pilot;
    EXPECT_STREQ("flying aircraft", pilot.description().c_str());
}

TEST(Job_Tests, Pilot_Hours) {
    Pilot pilot;
    EXPECT_EQ(40, pilot.hours());
}

TEST(Job_Tests, Pilot_WorkTest) {
    Pilot pilot;
    Mock_Printer* mock_printer = new Mock_Printer;
    pilot.set_printer(mock_printer);

    const string expected_output = k_description_prefix + pilot.description();
    pilot.DoWork();
    EXPECT_STREQ(expected_output.c_str(), mock_printer->m_ostream.str().c_str());
}

//test that we satisfy the requirement "call each of the available interface functions" of Job
TEST(Job_Tests, Exercise_Job_Class) {
    class Mock_Job : public Job {
    public:
        MOCK_METHOD(const string&, name, (), (const override));
        MOCK_METHOD(const string&, description, (), (const override));
        MOCK_METHOD(int, hours, (), (const override));
        MOCK_METHOD(void, DoWork, (), (const override));

    };
    vector<unique_ptr<Job>> jobs;

    Mock_Job* mock_job = new Mock_Job;
    jobs.emplace_back(mock_job);
    jobs.emplace_back(new Programmer);
    string testname = "TestName";
    string test_description = "doing professional things";
    EXPECT_CALL(*mock_job, name()).Times(AtLeast(1)).WillRepeatedly(Invoke([&]()-> const string& {return testname; }));
    EXPECT_CALL(*mock_job, description()).Times(AtLeast(1)).WillRepeatedly(Invoke([&]()-> const string& {return test_description; }));
    EXPECT_CALL(*mock_job, hours()).Times(AtLeast(1)).WillRepeatedly(Return(40));
    EXPECT_CALL(*mock_job, DoWork())
        .Times(AtLeast(1)).WillOnce(Invoke([&]() {
        //invoke the base class DoWork
        mock_job->Job::DoWork(); 
        }));


    exercise_job_classes(jobs);
}

TEST(Bounds_Tests, Lower) {
    EXPECT_FALSE(IsInBounds<uint32_t>(0, 1, 2));
}

TEST(Bounds_Tests, Upper) {
    EXPECT_FALSE(IsInBounds<uint32_t>(3, 1, 2));
}

TEST(Bounds_Tests, InBounds) {
    EXPECT_TRUE(IsInBounds<uint32_t>(2, 1, 3));
}

TEST(Bounds_Tests, LowerBoundary) {
    EXPECT_TRUE(IsInBounds<uint32_t>(1, 1, 3));
}

TEST(Bounds_Tests, UpperBoundary) {
    EXPECT_TRUE(IsInBounds<uint32_t>(3, 1, 3));
}
TEST(Bounds_Tests, Double_Boundary) {
    EXPECT_TRUE(IsInBounds<uint32_t>(2, 2, 2));
}


class Tests_ContainsTheString : public Test {
public:
    void SetUp()override {
        //set up 
    }

    void TearDown() {
        //tear down tasks
    }


    bool string_is_long(const string& str) {
        return min_len_for_long_string < str.size();
    }
    int min_len_for_long_string = 10;
};

TEST_F(Tests_ContainsTheString, empty_vector) {
    auto test_functor = [](const std::string& tested) { return tested == "test"; };
    EXPECT_EQ(0, ContainsTheString(test_functor, {}));
}

TEST_F(Tests_ContainsTheString, basic_test) {
    auto test_functor = [](const std::string& tested) { return tested == "test"; };
    EXPECT_EQ(1, ContainsTheString(test_functor, {"one", "two", "test"}));
}

TEST_F(Tests_ContainsTheString, output_test) {
    auto test_functor = [](const std::string& tested) { return tested == "test"; };
    EXPECT_EQ(1, ContainsTheString(test_functor, { "one", "two", "test" }));
}

TEST_F(Tests_ContainsTheString, many_matches) {
    auto test_functor = [](const std::string& tested) { return tested == "test"; };
    EXPECT_EQ(3, ContainsTheString(test_functor, { "test", "test", "test" }));
}

TEST_F(Tests_ContainsTheString, length_predicate) {
    EXPECT_EQ(2
        , ContainsTheString(std::bind(&Tests_ContainsTheString::string_is_long,this, std::placeholders::_1)
            , { "veryverylongstring", "somewhatlongstring", "shortstr" }));
}
