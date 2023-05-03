/*
 * Copyright (c) 2023, Eberty Alves
 * 
 * To run these tests from the command line, navigate to the directory containing this file and the CMakeLists.txt file
 * and run the command "cmake . && make" to build the project. Once the project has been built, you can run "ctest" to
 * execute all of the tests.
 *
 * To run these tests in Microsoft Visual Studio, open the Test Explorer by clicking Test > Test Explorer in the menu.
 * Then, select the tests you want to run and click the "Run" button to execute them.
 */

#include "Job.h"
#include "Programmer.h"
#include "Pilot.h"
#include "Utils.h"

#include <cassert>
#include <cmath>
#include <vector>
#include <string>

void testJob()
{
    Job job("Test Job", "Test Description", 10);
    assert(job.GetName() == "Test Job");
    assert(job.GetDescription() == "Test Description");
    assert(job.GetHoursRequired() == 10);

    job.SetName("New Name");
    assert(job.GetName() == "New Name");

    job.SetDescription("New Description");
    assert(job.GetDescription() == "New Description");

    job.SetHoursRequired(20);
    assert(job.GetHoursRequired() == 20);
}

void testProgrammer()
{
    Programmer programmer("Test Programmer", "Test Programming", 20, "C++");
    assert(programmer.GetName() == "Test Programmer");
    assert(programmer.GetDescription() == "Test Programming");
    assert(programmer.GetHoursRequired() == 20);
    assert(programmer.GetProgrammingLanguage() == "C++");

    programmer.SetName("New Name");
    assert(programmer.GetName() == "New Name");

    programmer.SetDescription("New Description");
    assert(programmer.GetDescription() == "New Description");

    programmer.SetHoursRequired(30);
    assert(programmer.GetHoursRequired() == 30);

    programmer.SetProgrammingLanguage("Java");
    assert(programmer.GetProgrammingLanguage() == "Java");
}

void testPilot()
{
    Pilot pilot("Test Pilot", "Test Flying", 30, "F-16");
    assert(pilot.GetName() == "Test Pilot");
    assert(pilot.GetDescription() == "Test Flying");
    assert(pilot.GetHoursRequired() == 30);
    assert(pilot.GetAircraftType() == "F-16");

    pilot.SetName("New Name");
    assert(pilot.GetName() == "New Name");

    pilot.SetDescription("New Description");
    assert(pilot.GetDescription() == "New Description");

    pilot.SetHoursRequired(40);
    assert(pilot.GetHoursRequired() == 40);

    pilot.SetAircraftType("F-22");
    assert(pilot.GetAircraftType() == "F-22");
}

void testSum()
{
    assert(Sum(1, 2, 3, 0) == 6);
    assert(Sum(0, 0, 0) == 0);
    assert(Sum(10, 20) == 30);

    assert(Sum(1.0, 2.5, 3.0) == 6.5);

    assert(std::fabs(Sum(1.0, 2.5, 3.0) - 6.5) < 0.0001);
    assert(std::fabs(Sum(0.0, 0.0, 0.0) - 0.0) < 0.0001);
    assert(std::fabs(Sum(1.1, 2.2) - 3.3) < 0.0001);

    assert(Sum(-1, -2, -3) == -6);
    assert(std::fabs(Sum(-1.1, -2.2, -3.3) - (-6.6)) < 0.001);

    assert(Sum(100000000L, 200000000L, 300000000L) == 600000000L);
    assert(std::fabs(Sum(1e9, 2e9, 3e9) - 6e9) < 0.0001);
}

void testSumHoursRequired()
{
    std::vector<Job *> jobs;
    jobs.push_back(new Programmer("Programmer", "developing", 40, "C++"));
    jobs.push_back(new Pilot("Pilot", "flying", 60, "F4U Corsair"));

    assert(SumHoursRequired(jobs[0], jobs[1]) != 40);
    assert(SumHoursRequired(jobs) == 100);

    jobs[0]->SetName("Senior Programmer");
    jobs[0]->SetHoursRequired(44);

    assert(SumHoursRequired(jobs[0], jobs[1]) == 104);
    assert(SumHoursRequired(jobs) == 104);

    jobs.push_back(new Job("Test Job 1", "Test Description 1", 5));
    jobs.push_back(new Job("Test Job 2", "Test Description 2", 15));
    jobs.push_back(new Job("Test Job 3", "Test Description 3", 20));

    assert(SumHoursRequired(jobs) == 144);
    assert(SumHoursRequired(jobs[2], jobs[3], jobs[4]) == 40);

    for (auto job : jobs)
    {
        delete job;
    }
}

void testIsInBounds()
{
    assert(IsInBounds<uint32_t>(503, 500, 599) == true);
    assert(IsInBounds<uint32_t>(400, 500, 599) == false);

    assert(IsInBounds<int>(10, 0, 100) == true);
    assert(IsInBounds<int>(-10, 0, 100) == false);

    assert(IsInBounds<double>(50.0, 0.0, 100.0) == true);
    assert(IsInBounds<double>(110.0, 0.0, 100.0) == false);

    assert(IsInBounds<float>(50.0, 0.0, 100.0) == true);
    assert(IsInBounds<float>(110.0, 0.0, 100.0) == false);

    assert(IsInBounds<char>('a', 'a', 'z') == true);
    assert(IsInBounds<char>('A', 'a', 'z') == false);

    bool exceptionThrown = false;
    try
    {
        IsInBounds<int>(10, 100, 0);
    }
    catch (std::invalid_argument &)
    {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
}

void testContainsTheString()
{
    auto theStrings = std::vector<std::string>{"one", "two", "test"};
    assert(ContainsTheString([](const std::string &tested) { return tested.length() == 3; }, theStrings) == 2);
    assert(ContainsTheString([](const std::string &tested) { return tested == "test"; }, theStrings) == 1);

    std::vector<std::string> strings = {"one", "two", "three"};
    assert(ContainsTheString([](const std::string &tested) { return tested == "four"; }, strings) == 0);
    assert(ContainsTheString([](const std::string &tested) { return tested != "four"; }, strings) == 3);
    assert(ContainsTheString([](const std::string &tested) { return tested == "one"; }, strings) == 1);
    assert(ContainsTheString([](const std::string &tested) { return tested == ""; }, strings) == 0);
    assert(ContainsTheString([](const std::string &tested) { return tested.empty(); }, strings) == 0);
    assert(ContainsTheString([](const std::string &tested) { return tested.size() > 3; }, strings) == 1);

    bool exceptionThrown = false;
    try
    {
        ContainsTheString(nullptr, strings);
    }
    catch (std::invalid_argument &)
    {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
}

int main()
{
    testJob();
    testProgrammer();
    testPilot();

    testSum();
    testSumHoursRequired();
    testIsInBounds();
    testContainsTheString();

    return 0;
}
