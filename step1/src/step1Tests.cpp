#include <gtest/gtest.h>
#include "step1Program.h"

TEST(step1Program, ContainsTheString)
{
    // Checks with lambda
    auto theStrings = vector<string>{"one", "two", "two", "test", "test", "test"};
    EXPECT_EQ(ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings), 3);

    // Checks with class method using std::function and bind - (Bonus)
    StringChecker string_checker;
    auto          hook = bind(&StringChecker::CheckIfContainsTWO, &string_checker, _1);
    EXPECT_EQ(ContainsTheString(hook, theStrings), 2);
}
TEST(step1Program, Jobs)
{
    // Create jobs using factory (Bonus)
    Job* prog           = JobFactory::CreateJob("Programmer");
    Job* pilot          = JobFactory::CreateJob("Pilot");
    Job* phone_operator = JobFactory::CreateJob("Phone Operator");

    EXPECT_NE(prog, nullptr);
    EXPECT_NE(pilot, nullptr);
    EXPECT_EQ(phone_operator, nullptr);

    vector<Job*> my_vect;
    my_vect.push_back(prog);
    my_vect.push_back(pilot);
    GTEST_ASSERT_EQ(my_vect.size(), 2);
    for (auto it = my_vect.begin(); it < my_vect.end(); it++)
    {
        //iterate with job* and compare strings with expected
        Job* curr_job = *it;

        EXPECT_PRED3([](string str, string s1, string s2) { return str == s1 || str == s2; }, curr_job->GetName(), "Programmer", "Pilot");
        EXPECT_PRED3([](string str, string s1, string s2) { return str == s1 || str == s2; }, curr_job->GetDescription(), "programming things", "piloting things");
        EXPECT_PRED3([](uint32_t str, uint32_t s1, uint32_t s2) { return str == s1 || str == s2; }, curr_job->GetReqHours(), 30, 40);
        testing::internal::CaptureStdout();
        curr_job->DoWork();
        string output = testing::internal::GetCapturedStdout();
        EXPECT_PRED3([](string str, string s1, string s2) { return str == s1 || str == s2; }, output, "My work involves programming things", "My work involves piloting things");
    }

    // cleanup
    delete prog;
    delete pilot;
}

TEST(step1Program, IsInBounds)
{

    //Test upper and lower limits, as well as numbers just outside the limits.
    EXPECT_FALSE(IsInBounds<uint32_t>(499, 500, 599));
    EXPECT_FALSE(IsInBounds<uint32_t>(600, 500, 599));
    EXPECT_TRUE(IsInBounds<uint32_t>(500, 500, 599));
    EXPECT_TRUE(IsInBounds<uint32_t>(599, 500, 599));
    //Test a value well enough within range
    EXPECT_TRUE(IsInBounds<uint32_t>(550, 500, 599));
}

TEST(step1Program, TemplVariadic_Specializ)
{
    // Create some variables to test specialization and variadic behavior
    auto        my_f   = 0.05f;
    auto        my_do  = 0.001;
    auto        my_num = 0;
    std::string my_str;
    //They are all considered zero
    EXPECT_TRUE(CheckIFZero(my_f, my_do, my_num, my_str));

    //Increase double value to be considered non zero and expect false
    my_do = 0.05;
    EXPECT_FALSE(CheckIFZero(my_f, my_do, my_num, my_str));

    //Decrease double value, but fill the string with some words. Expect false
    my_str = "Not zeroed";
    my_do  = 0.002;
    EXPECT_FALSE(CheckIFZero(my_f, my_do, my_num, my_str));
    //Clear the string and expect true
    my_str.clear();
    EXPECT_TRUE(CheckIFZero(my_f, my_do, my_num, my_str));

    //Test with fewer args
    EXPECT_TRUE(CheckIFZero(0, 0.02f));
    EXPECT_FALSE(CheckIFZero(0, 0.02));
}

TEST(step1Program, Singleton)
{
    //Instantiate the singleton with "20" as key
    SingleInstance& my_inst = SingleInstance::GetInstance(20);

    EXPECT_EQ(my_inst.GetKey(), 20);

    //Instantiate the singleton once again. However now its created already
    SingleInstance& my_sec_inst = SingleInstance::GetInstance(45);
    //Expected the key to be "20"
    EXPECT_NE(my_sec_inst.GetKey(), 45);
    EXPECT_EQ(my_sec_inst.GetKey(), 20);
}
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}