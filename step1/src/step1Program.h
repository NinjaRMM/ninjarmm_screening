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

/**
 * @brief       Base Job Class
 */
class Job
{
public:
    /**
     * @brief       Construct a new Job object
     */
    Job()
    {
    }

    /**
     * @brief       Destroy the Job object
     */
    ~Job()
    {
    }
    /**
     * @brief       Pure virtual Get Name method
     * @return      Name of the Job
     */
    virtual string GetName(void) = 0;
    /**
     * @brief       Pure virtual Get Description method
     * @return      Description of the Job
     */
    virtual string GetDescription(void) = 0;
    /**
     * @brief       Pure virtual Get Description method
     * @return      Number of hours 
     */
    virtual uint32_t GetReqHours(void) = 0;

    /**
     * @brief       Non-virtual Do Work method
     *              Calls the override method  
     */
    void DoWork(void)
    {
        cout << "My work involves " << GetDescription();
    }

private:
};

/**
 * @brief       Programmer subclass
 */
class Programmer : public Job
{
public:
    /**
     * @implements  GetName for Programmer subclass
     */
    string GetName(void) override
    {
        return "Programmer";
    }
    /**
     * @implements  GetDescription for Programmer subclass
     */
    string GetDescription(void) override
    {
        return "programming things";
    }
    /**
     * @implements  GetReqHours for Programmer subclass
     */
    uint32_t GetReqHours(void) override
    {
        return 30;
    }

private:
};

/**
 * @brief       Pilot subclass
 */
class Pilot : public Job
{
public:
    /**
     * @implements  GetName for Pilot subclass
     */
    string GetName(void) override
    {
        return "Pilot";
    }
    /**
     * @implements  GetDescription for Pilot subclass
     */
    string GetDescription(void) override
    {
        return "piloting things";
    }
    /**
     * @implements  GetReqHours for Pilot subclass
     */
    uint32_t GetReqHours(void) override
    {
        return 40;
    }

private:
};

/**
 * @brief       Singleton class example
 */
class SingleInstance
{
public:
    /**
     * @brief       Get the singleton instance
     * @param[in]   key: Used to create the first time
     * @return      Reference to the singleton classs=
     */
    static SingleInstance& GetInstance(uint32_t key)
    {
        // The key parameter was just created to show that the class is only
        // instantiated once
        static SingleInstance instance(key);
        return instance;
    }
    /**
     * @brief       Get the key object set when the singleton is created
     * @return      key value
     */
    uint32_t GetKey(void)
    {
        return my_key;
    }

    SingleInstance(SingleInstance& n)     = delete; // Delete copy constructor
    void operator=(const SingleInstance&) = delete; // Delete assign operator

private:
    /**
     * @brief       Construct the singleton. (Private)
     * @param[in]   key: Just initializes the key attribute.
     * 
     */
    SingleInstance(uint32_t key) : my_key(key){};

    uint32_t my_key; ///< key used to test the singleton
};

class JobFactory
{
public:
    static Job* CreateJob(std::string job_str)
    {
        Job* new_job = nullptr;

        if (job_str == "Pilot")
        {
            new_job = new Pilot();
        }
        else if (job_str == "Programmer")
        {
            new_job = new Programmer();
        }
        return new_job;
    }
};

template <typename T>
bool IsInBounds(T val, T low, T high)
{
    bool ret = false;
    cout << "Requested value: " << val << endl;
    cout << "Begin: " << low << endl;
    cout << "End: " << high << endl;
    if ((val >= low) && (val <= high))
    {
        ret = true;
        cout << "Within range" << endl;
    }
    else
        cout << "Outside range" << endl;

    return ret;
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

template <typename T>
bool CheckIFZero(T var)
{
    return var == 0;
}
template <typename T, typename... types>
bool CheckIFZero(T var, types... vars)
{
    return CheckIFZero(var) && CheckIFZero(vars...);
}
template <>
bool CheckIFZero<float>(float var)
{
    return (var < 0.09f) && (var > -0.09f);
}
template <>
bool CheckIFZero<double>(double var)
{
    return (var < 0.009) && (var > -0.009f);
}
template <>
bool CheckIFZero<std::string>(string var)
{
    return var == "";
}
