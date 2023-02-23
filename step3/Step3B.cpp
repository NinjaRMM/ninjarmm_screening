// Step3B.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//For unit testing code, please see file UnitTest2.cpp

#include <iostream>
#include <list>
#include <functional>
#include <vector>

class Job {
protected:
    std::string name;
    std::string description;
    int num_req_hours; //the number of hours required by the job

public:
    Job(std::string new_name = "(not set)", std::string new_description = "(not set)", int num_req_hours = 0);
    std::string GetName();
    std::string GetDescription();
    int GetNumReqHours();
    void DoWork();
    ~Job();
};

Job::Job(std::string new_name, std::string new_description, int new_req_hours)
{
    name = new_name;
    description = new_description;
    num_req_hours = new_req_hours;
}

std::string Job::GetName()
{
    return name;
}

std::string Job::GetDescription()
{
    return description;
}

int Job::GetNumReqHours()
{
    return num_req_hours;
}

void Job::DoWork()
{
    std::cout << "My work involves " << description << "\n";
}

Job::~Job()
{

}

class Programmer :public Job
{
public:
    Programmer(std::string new_name = "(not set)", std::string new_description = "(not set)", int new_req_hours = 0) :Job(new_name, new_description, new_req_hours)
    {
    }
    ~Programmer();
};

Programmer::~Programmer()
{
}

class Pilot :public Job
{
public:
    Pilot(std::string new_name = "(not set)", std::string new_description = "(not set)", int new_req_hours = 0) :Job(new_name, new_description, new_req_hours)
    {
    }
    ~Pilot();
};

Pilot::~Pilot()
{
}

template <class myType>
myType IsInBounds(myType httpResponseCode, myType minimum, myType maximum)
{
    if ((httpResponseCode >= minimum) && (httpResponseCode <= maximum))
        return true;
    else
        return false;
}

int ContainsTheString(std::function<bool(std::string& testInputString)> func, std::vector<std::string> testStringVector)
{
    int containsCnt = 0;
    //iterate through the testStringVector
    std::vector<std::string>::iterator it;
    for (it = testStringVector.begin(); it != testStringVector.end(); ++it)
        //for each string in testStringVector, see if it matches the testInputString by calling the func
        //if so, then add it to the count
        if (func(*it))
        {
            containsCnt++;
        }
    return containsCnt;
}

int main()
{
    //Create Job instances, add them to a list, and iterate through them
    Programmer testProgrammer("Joe", "Programmer", 8);
    Pilot testPilot("Milo", "Pilot", 16);

    std::list<Job> job_list;
    job_list.push_back(testProgrammer);
    job_list.push_back(testPilot);

    std::list<Job>::iterator it;
    for (it = job_list.begin(); it != job_list.end(); ++it)
    {
        std::string job_name = it->GetName();
        std::cout << "Job name is " << job_name << "\n";
        std::string job_descr = it->GetDescription();
        std::cout << "Job description is " << job_descr << "\n";
        int job_hours = it->GetNumReqHours();
        std::cout << "Job's required hours is " << job_hours << "\n";
        it->DoWork();
        std::cout << "\n";
    }
    job_list.clear();

    //Create a template function to test if values are in a range
    bool is_in_bounds = IsInBounds<uint32_t>(501, 500, 599);
    std::cout << "Result from calling IsInBounds with inputs 501, 500, 599, and type uint32_t is: ";
    if (is_in_bounds)
    {
        std::cout << "True\n\n";
    }
    else
    {
        std::cout << "False\n\n";
    }

    //Create a function that will take a vector of strings and a test function, It should return how many items in the collection matched the test. Then print out the result.
    std::vector<std::string> theStrings;
    theStrings.push_back("one");
    theStrings.push_back("two");
    theStrings.push_back("test");
    int contains_string_count = ContainsTheString([](const std::string& tested) { std::string test_string = "test"; return (tested == test_string); }, theStrings);
    std::cout << "Num of strings with value " << "test in vector is " << contains_string_count << "\n";
    theStrings.clear();

    std::cout << "\n";
}