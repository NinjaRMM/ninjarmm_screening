/*
 * Copyright (c) 2023, Eberty Alves
 */

#include "Job.h"
#include "Programmer.h"
#include "Pilot.h"
#include "Utils.h"

#include <iostream>
#include <memory>
#include <vector>

int main(int argc, char *argv[])
{
    {
        // Print information about jobs (a to g)
        std::vector<std::unique_ptr<Job>> jobs;
        jobs.push_back(std::make_unique<Programmer>("Programmer 1", "developing software", 40, "C++"));
        jobs.push_back(std::make_unique<Pilot>("Pilot 1", "flying airplanes", 30, "F4U Corsair"));
        jobs.push_back(std::make_unique<Job>("Job 3", "performing a generic job", 20));

        for (const auto &job : jobs)
        {
            std::cout << "Job: " << job->GetName() << std::endl;
            std::cout << "Description: " << job->GetDescription() << std::endl;
            std::cout << "Hours Required: " << job->GetHoursRequired() << std::endl;
            job->DoWork();
            std::cout << std::endl;
        }

        // Calculate the hours required for jobs (j)
        std::cout << "-----------------\n\n";

        std::cout << "Hours required for 'Programmer 1' and 'Pilot 1' (before changes): "
                  << SumHoursRequired(jobs[0].get(), jobs[1].get()) << std::endl;

        jobs[0]->SetName("Programmer 2");
        jobs[0]->SetHoursRequired(50);

        std::cout << "Total hours required for all jobs (after changes): " << SumHoursRequired(jobs) << std::endl;
    }

    // Check if a value is within a range (h)
    std::cout << "\n-----------------\n\n";

    uint32_t httpResponseCode = 501;
    std::cout << "HTTP Response Code: " << httpResponseCode << std::endl;
    std::cout << "Is it within the range 500-599? "
              << std::boolalpha
              << IsInBounds<uint32_t>(httpResponseCode, 500, 599)
              << std::endl;

    // Count the number of strings in a vector that match a certain criterion (i)
    std::cout << "\n-----------------\n\n";

    auto theStrings = std::vector<std::string>{"one", "two", "test"};
    auto count = ContainsTheString([](const std::string &tested) { return tested == "test"; }, theStrings);
    std::cout << "Number of strings that contain 'test': " << count << std::endl;

    return 0;
}
