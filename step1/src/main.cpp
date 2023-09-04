#include "functions.h"
#include "Job.h"
#include "Pilot.h"
#include "Programmer.h"
#include <vector>
#include <iostream>
#include <memory>
#include <utility>

int main(int argc, char* argv[])
{
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Code related to \"a, b, c, d, e, f, g\"" << std::endl << std::endl;

    auto pilot1 = std::make_shared<Pilot>("Pilot for ACME Airlines", 10000U);
    auto pilot2 = std::make_shared<Pilot>("Pilot for Lunar Rovers", 1000000U);
    auto programmer1 = std::make_shared<Programmer>("C++ Programmer", "C++, CMake");
    auto programmer2 = std::make_shared<Programmer>("Java Programmer", "Java, SpringBoot");

    auto jobs = std::vector<std::shared_ptr<Job>>{pilot1, pilot2, programmer1, programmer2};

    for (auto &job : jobs)
    {
        std::cout << "Job Name: " << job->GetName() << std::endl;
        std::cout << "Description: " << job->GetDescription() << std::endl;
        std::cout << "Work Hours: " << std::to_string(job->GetWorkHours()) << std::endl;
        std::cout << "Minimum Requirements: " << job->MinimumRequirements() << std::endl;
        std::cout << std::endl;
    }


    std::cout << std::endl << "-------------------------------------" << std::endl;
    std::cout << "Code related to \"h\"" << std::endl << std::endl;

    IsInBoundsPrinter(IsInBounds<uint32_t>(3, 1, 4));
    IsInBoundsPrinter(IsInBounds<uint32_t>(1, 2, 3));
    IsInBoundsPrinter(IsInBounds<char>('o', 'a', 'z'));
    IsInBoundsPrinter(IsInBounds<char>('O', 'a', 'z'));


    std::cout << std::endl << "-------------------------------------" << std::endl;
    std::cout << "Code related to \"i\"" << std::endl << std::endl;
   
    auto theStrings = std::vector<std::string>{"one", "two", "test", "testing", "autotest", "this is a test", "Test", "test"};
    auto count = ContainsTheString([](const std::string &tested) { return tested == "test"; }, theStrings);
    std::cout << "Contains the string \"test\": " << count <<  std::endl;


    std::cout << std::endl << "-------------------------------------" << std::endl;
    std::cout << "Code related to \"j\"" << std::endl << std::endl;

    PrintInReverseOrder(1, 2, 3, 4, 5, 6, 7);
    PrintInReverseOrder('a', 'b', 'c', 'd', 'e', 'f');


    std::cout << std::endl << "-------------------------------------" << std::endl;
    std::cout << "Code related to \"k\"" << std::endl << std::endl;

    std::cout << ">>>>> The code of the section k is in the file test.cpp. It needs the GTest library to run. <<<<<\n\n\n";

  
    return 0;
}