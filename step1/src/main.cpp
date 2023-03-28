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
#include <vector>
#include <iostream>

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

    return 0;
}