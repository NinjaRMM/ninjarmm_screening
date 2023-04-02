#include<iostream>
#include<vector>
#include<string>
#include<functional>

#include <ctime>

#include "jobs.h"

using namespace std;

const char * Job::getJobName()
{
    return jobName;
}

const char * Job::getJobDescription()
{
    return jobDescription;
}

int Job::getJobHours()
{
    return jobHours;
}

void Job::DoWork()
{
    cout << "My work involves " << jobDescription << endl;
};


template<typename T>
bool IsInBounds(T code, T lowerBound, T upperBound)
{
    return (code >= lowerBound && code <= upperBound);
}

template<typename T, typename... Args>
void testIsInBounds(T lowerBound, T upperBound, Args... args)
{
    array<int, sizeof...(args)> codes = {args...};
    for (const auto& code : codes)
    {
        cout << code << " is " << (IsInBounds<T>(code, lowerBound, upperBound) ? "in range" : "out of range") << endl;
    }
}

template<typename T>
int ContainsTheString(function<bool(const T&)> testFunc, const vector<T>& strings) {
    int count = 0;

    for (auto it = strings.begin(); it != strings.end(); it++)
    {
        if (testFunc(*it))
        {
            count++;
        }
    }

    return count;
}

void testContainsTheString()
{
    vector<string> theStrings;
    srand(time(NULL));

    int TestedAdded = 0;
    for (int i = 0; i < 10; i++)
    {
        int random_number = rand() % 10 + 1;
        if (random_number <= 4)
        {
            theStrings.emplace_back("test");
            TestedAdded++;
        }
        else
        {
            theStrings.emplace_back("some string");
        }
    }
    cout << TestedAdded << " tested added to the list" << endl;
    int count = ContainsTheString<string>([](const string& tested) { return tested == "test"; }, theStrings);
    cout << (TestedAdded == count ? "test passed " : "test failed ") << count << " strings tested" << endl;
}

int main(int argc, char * argv[])
{
    // Jobs
    vector<Job*> jobs;

    Programmer * programmer = new Programmer();
    jobs.emplace_back(programmer);

    Pilot * pilot = new Pilot();
    jobs.emplace_back(pilot);

    while (jobs.size() > 0)
    {
        auto it = jobs.begin();
        cout << (*it)->getJobName() << endl;
        cout << (*it)->getJobDescription() << endl;
        cout << (*it)->getJobHours() << endl;
        (*it)->DoWork();

        delete(*it);
        jobs.erase(it);
    }

    // is in bound
    uint32_t httpResonseCode = 505, lower = 500, upper = 599;
    cout << endl << "testing if reason code " << httpResonseCode << " is in between " << lower << " and " << upper << endl;
    if (IsInBounds<uint32_t>(httpResonseCode, lower, upper))
    {
        cout << "reason code is in bound" << endl;
    }
    else
    {
        cout << "reason code is out of bounds" << endl;
    }

    // unit test several codes
    testIsInBounds(lower, upper, 100, 500, 505, 600, 900);


    // vector of strings
    auto theStrings = vector<string> { "one", "two", "test"};
    auto count = ContainsTheString<string>([](const string& tested) { return tested == "test"; }, theStrings);

    cout << endl << count << " strings tested" << endl;

    testContainsTheString();

    return 0;

};