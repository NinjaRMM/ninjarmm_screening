/**
 * @file main.cpp
 * @author Guilherme El Kadri Ribeiro (guilhermeelkadri@gmail.com)
 * @brief Test material NinjaRMM - Step 1
 * @version 0.1
 * @date 2023-06-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "step1.hpp"

int main() 
{
    std::vector<Job*> jobs;
    
    Job* programmer = new Programmer();
    Job* pilot = new Pilot();
    
    jobs.push_back(programmer);
    jobs.push_back(pilot);
    
    for (const auto& job : jobs) 
    {
        job->action1();
        job->action2();
        job->action3();
        job->action4();
    }

    // Test IsInBounds function
    uint32_t httpResponseCode = 503;
    bool isInBounds = IsInBounds<uint32_t>(httpResponseCode, 500, 599);
    std::cout << "IsInBounds: httpResponseCode=" << httpResponseCode << ", result=" << (isInBounds ? "true" : "false") << std::endl;

    // Test ContainsTheString function
    std::vector<std::string> theStrings = { "one", "two", "test" };
    int count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    std::cout << "ContainsTheString: count=" << count << std::endl;

    // Clean up dynamically allocated memory
    delete programmer;
    delete pilot;

    return 0;
}