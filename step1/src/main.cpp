
/**
 * @file main.cpp
 *
 * @brief Ninja one screanning app.
 *
 * @author   gustavo.garcia
 * @date     Apr 02, 2023
 * @revision 1
 *
 */

/* INCLUDES ******************************************************************/

#include <iostream>
#include <string>
#include <vector>

#include "Pilot.hpp"
#include "Programmer.hpp"
#include "Utils.hpp"

template<typename T>
T sum(const T& arg)
{
    return arg;
}

template<typename T, typename... Args>
T sum(const T& arg, const Args&... args)
{
    return arg + sum(args...);
}

/* MAIN  *********************************************************************/

int main() {
    std::vector<IJob*> jobs = {new Programmer(), new Pilot()};

    for (const auto& job : jobs) {
        std::cout << "Job Name: " << job->GetName() << std::endl;
        std::cout << "Job Description: " << job->GetDescription() << std::endl;
        std::cout << "Hours Required: " << job->GetHoursRequired() << std::endl;
        job->DoWork();
        std::cout << std::endl;
    }

    for (const auto& job : jobs) {
        delete job;
    }

    uint32_t httpResponseCode = 501;
    bool isInBounds = IsInBounds<uint32_t>(httpResponseCode, 500, 599);
    std::cout << "httpResponseCode: " << httpResponseCode << std::endl;
    std::cout << "Is in bounds: " << std::boolalpha << isInBounds << std::endl;


    auto theStrings = std::vector<std::string>{"one", "two", "test"};
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    std::cout << "Number of times the word 'test' is repeated: " << count << std::endl;

    // Variadic Template example
    std::cout << "Sum using variadic Template: " << sum(1, 2, 3, 4, 5) << std::endl;

    return 0;
}
