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
    for (auto it = my_vect.begin(); it < my_vect.end(); it++)
    {
        Job* curr_job = *it;

        EXPECT_PRED3([](string str, string s1, string s2) { return str == s1 || str == s2; }, curr_job->GetName(), "Programmer", "Pilot");
        EXPECT_PRED3([](string str, string s1, string s2) { return str == s1 || str == s2; }, curr_job->GetDescription(), "programming things",
                     "piloting things");
        EXPECT_PRED3([](uint32_t str, uint32_t s1, uint32_t s2) { return str == s1 || str == s2; }, curr_job->GetReqHours(), 30, 40);
        testing::internal::CaptureStdout();
        curr_job->DoWork();
        string output = testing::internal::GetCapturedStdout();
        EXPECT_PRED3([](string str, string s1, string s2) { return str == s1 || str == s2; }, output, "My work involves programming things",
                     "My work involves piloting things");
    }

    delete prog;
    delete pilot;
}

TEST(step1Program, IsInBounds)
{
    EXPECT_FALSE(IsInBounds<uint32_t>(499, 500, 599));
    EXPECT_FALSE(IsInBounds<uint32_t>(600, 500, 599));
    EXPECT_TRUE(IsInBounds<uint32_t>(500, 500, 599));
    EXPECT_TRUE(IsInBounds<uint32_t>(500, 500, 599));
    EXPECT_TRUE(IsInBounds<uint32_t>(599, 500, 599));
    EXPECT_TRUE(IsInBounds<uint32_t>(550, 500, 599));
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}