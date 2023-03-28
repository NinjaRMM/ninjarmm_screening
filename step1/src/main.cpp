/**
 * @defgroup    Main This is the main source module group
 * @ingroup     Main
 * @{
 * @file        main.cpp
 * @brief       Source file to test the step1
 * @version     0.1
 * @date        2023-03-28
 * @copyright   Copyright (c) 2023
 * @author      Henrique Awoyama Klein (henryaklein@gmail.com)
 */
#include "step1Program.h"

int main(int argc, char** argv)
{
    Job* prog  = new Programmer();
    Job* pilot = new Pilot();

    vector<Job*> my_vect;
    my_vect.push_back(prog);
    my_vect.push_back(pilot);
    for (auto it = my_vect.begin(); it < my_vect.end(); it++)
    {
        Job* curr_job = *it;

        cout << curr_job->GetName() << endl;
        cout << curr_job->GetDescription() << endl;
        cout << curr_job->GetReqHours() << endl;
        curr_job->DoWork();
    }
    IsInBounds<uint32_t>(499, 500, 599);
    IsInBounds<uint32_t>(600, 500, 599);
    IsInBounds<uint32_t>(500, 500, 599);
    IsInBounds<uint32_t>(599, 500, 599);
    IsInBounds<uint32_t>(550, 500, 599);

    auto theStrings = vector<string>{"one", "two", "two", "test", "test", "test"};

    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);

    cout << "number of \"test\" strings " << count << endl;

    StringChecker string_checker;
    auto          hook = bind(&StringChecker::CheckIfContainsTWO, &string_checker, _1);
    count              = ContainsTheString(hook, theStrings);

    cout << "number of \"two\" strings " << count << endl;
    delete prog;
    delete pilot;
    return 0;
}