#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

#include "Programmer.h"
#include "Pilot.h"

#include "TemplateSpecialization.h"

void JobTest();
void BoundsTest();
void StringMatchTest();
void TemplateSpecializationTest();

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
    TemplateSpecializationTest();

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
            if (IsInBounds(code, min, max))
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
    auto testerFn = [](const std::string& tested)
    {
        return tested == "test";
    };

    auto theStrings = std::vector<std::string>{"one", "two", "test", "four", "test", "six", "test"};
    auto count = ContainsTheString(testerFn, theStrings);
    std::cout << "\"test\" appeared " << count << " times" << std::endl; // 3

    theStrings = std::vector<std::string>{ "one", "two", "three", "four", "five", "six", "seven" };
    count = ContainsTheString(testerFn, theStrings);
    std::cout << "\"test\" appeared " << count << " times" << std::endl; // 0

    theStrings = std::vector<std::string>{};
    count = ContainsTheString(testerFn, theStrings);
    std::cout << "\"test\" appeared " << count << " times" << std::endl; // 0

    std::cout << "----" << std::endl;
}

int ContainsTheString(std::function<bool(const std::string&)> tester,
    const std::vector<std::string>& strings)
{
    auto sum = 0;

    std::for_each(strings.begin(), strings.end(), [&](const std::string& s)
    {
        if (tester(s))
        {
            ++sum;
        }
    });

    return sum;
}

void TemplateSpecializationTest()
{
    auto a = 10;
    auto b = 1.0;
    auto c = "hello";

    foo("hello");   // 4 - T is char*. C-style array is a pointer to a character
    foo(10);        // 7 - non-template version has higher priority over template version.
    foo(1.0);       // 3 - T is double. Specialized template has higher precedence over unspecified T.
    foo(a);         // 7 - non-template version. same as foo(10)
    foo(b);         // 3 - T is double. same with foo(1.0)
    foo(&c);        // 4 - T is char*. so a must be char**
    foo(&a);        // 5 - T is int*. Specialized template has higher precedence over unspecified T.
    foo(&b);        // 6 - T is double*. Specialized template has higher precedence over unspecified T.
    foo<int>(2);    // 2 - T is int. Explicit call to template specialization.

    // Order of prioritization:
    //  non-template
    //  template specialization
    //  template with no specialization
}