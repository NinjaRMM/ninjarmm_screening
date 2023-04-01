#include<iostream>
#include<vector>
#include<string>
#include<functional>

#include "jobs.h"

using namespace std;

const char * Job::getJobName()
{
    return _name;
}

const char * Job::getJobDescription()
{
    return _description;
}

int Job::getJobHours()
{
    return _hours;
}

void Job::DoWork()
{
    cout << "My work involves " << _description << endl;
};

template<typename T>
bool IsInBounds(T code, T lowerBound, T upperBound)
{
    return (code >= lowerBound && code <= upperBound);
}

template<typename T>
int ContainsTheString(function<bool(const T&)> testFunc, const vector<T>& strings) {
    int count = 0;

    auto it = strings.begin();
    while (it != strings.end())
    {
        if (testFunc(*it))
        {
            count++;
        }
        it++;
    }

    return count;
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
    uint32_t httpResonseCode = 505;
    cout << "testing if reason code " << httpResonseCode << " is in between 500 and 599" << endl;
    if (IsInBounds<uint32_t>(httpResonseCode, 500, 599))
    {
        cout << "reason code is in bound" << endl;
    }
    else
    {
        cout << "reason code is out of bounds" << endl;
    }


    // vector of strings
    auto theStrings = vector<string> { "one", "two", "test"};
    auto count = ContainsTheString<string>([](const string& tested) { return tested == "test"; }, theStrings);

    cout << count << " strings tested" << endl;

    return 0;

};