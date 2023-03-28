/**
 * @defgroup    step1Program.h Description
 * @ingroup     step1Program.h
 * @{
 * @file        step1Program.h
 * @brief       Header file of the step1 assignment
 * @version     0.1
 * @date        2023-03-28
 * @copyright   Copyright (c) 2023
 * @author      Henrique Awoyama Klein (henryaklein@gmail.com)
 */
#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>
#include <functional>

using namespace std;
using namespace placeholders;

class Job
{
public:
    Job(/* args */)
    {
    }

    ~Job()
    {
    }

    virtual string   GetName(void)        = 0;
    virtual string   GetDescription(void) = 0;
    virtual uint32_t GetReqHours(void)    = 0;
    void             DoWork(void)
    {
        cout << "My work involves " << GetDescription();
    }

private:
    /* data */
};

class Programmer : public Job
{
public:
    string GetName(void) override
    {
        return "Programmer";
    }
    string GetDescription(void) override
    {
        return "programming things";
    }
    uint32_t GetReqHours(void) override
    {
        return 30;
    }

private:
};
class Pilot : public Job
{
public:
    string GetName(void) override
    {
        return "Pilot";
    }
    string GetDescription(void) override
    {
        return "piloting things";
    }
    uint32_t GetReqHours(void) override
    {
        return 40;
    }

private:
};

template <typename T>
void IsInBounds(T val, T low, T high)
{
    cout << "Requested value: " << val << endl;
    cout << "Begin: " << low << endl;
    cout << "End: " << high << endl;
    if ((val >= low) && (val <= high))
        cout << "Within range" << endl;
    else
        cout << "Outside range" << endl;
}

size_t ContainsTheString(function<bool(const string&)> foo, vector<string>& strs)
{
    size_t times = 0;
    for (auto it = strs.begin(); it < strs.end(); it++)
    {
        if (foo(*it) == true)
            times++;
    }
    return times;
}

class StringChecker
{
public:
    bool CheckIfContainsTWO(const std::string& teststr)
    {
        return teststr == "two";
    }

private:
};
