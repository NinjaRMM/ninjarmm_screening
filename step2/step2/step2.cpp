// step2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <vector>

#include "Programmer.h"
#include "Pilot.h"

std::vector<std::unique_ptr<Job>> CreateJobs();
void printJob(const std::unique_ptr<Job>& job);

int main()
{
    auto jobCollection = CreateJobs();

    std::for_each(jobCollection.begin(), jobCollection.end(), printJob);

    return 0;
}

std::vector<std::unique_ptr<Job>> CreateJobs()
{
    auto jobs = std::vector<std::unique_ptr<Job>>();

    jobs.push_back(std::make_unique<Programmer>());
    jobs.push_back(std::make_unique<Pilot>());

    return jobs;
}

void printJob(const std::unique_ptr<Job>& job)
{
    std::cout << "Job Name: " << job->GetName() << std::endl;
    std::cout << "Description" << job->GetDescription() << std::endl;
    std::cout << "Required Number of Hours" << job->GetRequiredHours() << std::endl;
    std::cout << job->DoWork() << std::endl;
    std::cout << "----" << std::endl;
}
