#include <gtest/gtest.h>
#include "step1Program.h"

TEST(step1Program, ContainsTheString)
{
    auto theStrings = vector<string>{"one", "two", "two", "test", "test", "test"};
    EXPECT_EQ(ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings), 3);
    StringChecker string_checker;

    auto hook = bind(&StringChecker::CheckIfContainsTWO, &string_checker, _1);
    EXPECT_EQ(ContainsTheString(hook, theStrings), 2);
}
TEST(step1Program, Jobs)
{
    Job* prog  = new Programmer();
    Job* pilot = new Pilot();

    EXPECT_NE(prog, nullptr);
    EXPECT_NE(pilot, nullptr);

    vector<Job*> my_vect;
    my_vect.push_back(prog);
    my_vect.push_back(pilot);
    GTEST_ASSERT_EQ(my_vect.size(), 2);

    Job*   gen_job = my_vect.at(0);
    string s1      = "Programmer";
    GTEST_ASSERT_EQ(gen_job->GetName(), "Programmer");
    GTEST_ASSERT_EQ(gen_job->GetDescription(), "I program things");
    GTEST_ASSERT_EQ(gen_job->GetReqHours(), 30);

    gen_job = my_vect.at(1);
    GTEST_ASSERT_EQ(gen_job->GetName(), "Pilot");
    GTEST_ASSERT_EQ(gen_job->GetDescription(), "I pilot things");
    GTEST_ASSERT_EQ(gen_job->GetReqHours(), 40);
    //EXPECT_STREQ(gen_job->DoWork(), "My work involves Programming");
    delete prog;
    delete pilot;
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}