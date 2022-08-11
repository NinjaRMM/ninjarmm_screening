// Part3Assignment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;


class Job 
{
protected: 

    string m_name;
    string m_description;
    int    m_hours;

public:

    Job();
    ~Job() {};
    virtual string GetName() { return m_name; }
    virtual string GetDescription() { return m_description; }
    virtual int GetHours() { return m_hours; }
    virtual string DoWork() { return "My work involves " + m_description; }
};

Job::Job()
{
    m_name = "None";
    m_description = "staying home";
    m_hours = 24 * 7;
}


class Programmer: public Job
{
public: 

    Programmer();
    ~Programmer() {};

};

Programmer::Programmer()
{
    m_name = "Programmer";
    m_description = "writing code";
    m_hours = 40;
}

class Pilot : public Job
{
public:

    Pilot();
    ~Pilot() {};

};

Pilot::Pilot()
{
    m_name = "Pilot";
    m_description = "flying planes";
    m_hours = 25;
}


// h.Create a template function that will test if the provided values are within a range.
// Call the function and print out the inputs and result.
template <typename T> 
bool IsInBounds(T tester, T lower, T upper)
{
    if (tester >= lower && tester <= upper)
        return true;

    return false;
}

//i.Create a function that will take a vector of strings and a test function, 
//It should return how many items in the collection matched the test.Then print out the result.
template <typename P>
int PerformTest(P testFunc, vector<string>& theStrings)
{
    int count = 0;

    for (int i = 0; i < theStrings.size(); i++)
    {
        if (testFunc(theStrings[i]))
            count++;
   }
    return count;
}

int main()
{
    vector<Job*> pJobs;

    Job* newJob = new Job();
    pJobs.push_back(newJob);

    Programmer* newProgrammer = new Programmer();
    pJobs.push_back(newProgrammer);

    Pilot* newPilot = new Pilot();
    pJobs.push_back(newPilot);

    for (vector<Job*>::iterator i = pJobs.begin(); i < pJobs.end(); i++)
    {
        Job* pObj = *i;
        cout << pObj->GetName() << endl;
        cout << pObj->GetDescription() << endl;
        cout << pObj->DoWork() << " for " << pObj->GetHours() << " hours a week" << endl << endl;
    }
    for (vector<Job*>::iterator i = pJobs.begin(); i < pJobs.end(); i++) { delete* i; }

    uint32_t httpResponseCode = 600; // Get the response from somewhere
    uint32_t lowerRange = 500;
    uint32_t upperRange = 599;

    if (IsInBounds<uint32_t>(httpResponseCode, lowerRange, upperRange))
        cout << httpResponseCode << " is within " << lowerRange << " and " << upperRange << endl;
    else
        cout << httpResponseCode << " is NOT within " << lowerRange << " and " << upperRange << endl;

    vector<string> theStrings{ "test", "one", "two",  "test"};
    auto count = PerformTest([](const std::string& tested) { return tested == "test"; }, theStrings);

    cout << "The test passed " << count << " times" << endl;

    return 0;

}

