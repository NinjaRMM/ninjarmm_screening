// step2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

#include "Programmer.h"
#include "Pilot.h"

void JobTest();
void BoundsTest();
void StringMatchTest();

int ContainsTheString(std::function<bool(const std::string&)> tester,
    const std::vector<std::string>& strings);

template <typename T>
bool IsInBounds(T code, T min, T max)
{
    return code >= min && code <= max;
}

int main()
{
    JobTest();
    BoundsTest();
    StringMatchTest();

    _CrtDumpMemoryLeaks();
    return 0;
}

void JobTest()
{
    auto programmer = std::make_unique<Programmer>();
    auto pilot = std::make_unique<Pilot>();

    auto jobs = std::vector<std::unique_ptr<Job>>();
    jobs.emplace_back(std::move(programmer));
    jobs.emplace_back(std::move(pilot));

    for (const auto& i : jobs)
    {
        std::cout << "Job Name: " << i->GetName() << std::endl;
        std::cout << "Description: " << i->GetDescription() << std::endl;
        std::cout << "Required Number of Hours: " << i->GetRequiredHours() << std::endl;
        std::cout << i->DoWork() << std::endl;
        std::cout << "----" << std::endl;
    }

    jobs.clear();
}

void BoundsTest()
{
    const auto boundsTester = [](uint32_t code, uint32_t min, uint32_t max)
        {
            if (IsInBounds<uint32_t>(code, 500, 599))
            {
                std::cout << code << " is within bounds." << std::endl;
            }
            else
            {
                std::cout << code << " is out of bounds." << std::endl;
            }
        };

    auto httpResponseCode = uint32_t(499);
    boundsTester(httpResponseCode, 500, 599); // out of bounds
    httpResponseCode = uint32_t(500);
    boundsTester(httpResponseCode, 500, 599); // within bounds
    httpResponseCode = uint32_t(505);
    boundsTester(httpResponseCode, 500, 599); // within bounds
    httpResponseCode = uint32_t(599);
    boundsTester(httpResponseCode, 500, 599); // within bounds
    httpResponseCode = uint32_t(600);
    boundsTester(httpResponseCode, 500, 599); // out of bounds

    std::cout << "----" << std::endl;
}

void StringMatchTest()
{
    auto theStrings = std::vector<std::string>{"one", "two", "test", "four", "test", "six", "test"};
    auto count = ContainsTheString([](const std::string& tested)
        {
            return tested == "test";
        }, theStrings);

    std::cout << "\"test\" appeared " << count << " times" << std::endl;
    std::cout << "----" << std::endl;
}

int ContainsTheString(std::function<bool(const std::string&)> tester,
    const std::vector<std::string>& strings)
{
    auto sum = 0;

    for (auto i = strings.begin(); i != strings.end(); ++i)
    {
        if (tester(*i))
        {
            ++sum;
        }
    }

    return sum;
}
