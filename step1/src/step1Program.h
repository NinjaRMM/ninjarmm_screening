/**
 * @file        step1Program.h
 * @brief       Header file of the step1 assignment
 * @version     1.0
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
 * @brief       Singleton class example (Bonus)
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

/**
 * @brief       Simple factory to create jobs (Bonus)
 */
class JobFactory
{
public:
    /**
     * @brief       Method to create jobs according to string parameter
     * @param[in]   job_str: Name of job
     * @return      Job instance if not nullptr 
     */
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

/**
 * @brief       Check if a value is in bounds.
 *              Goes from low to high-1
 * @param[in]   val: Value to be checked
 * @param[in]   low: Lowest value possible
 * @param[in]   high: One after the highest value possible
 * @return      true if the value is within limits, false otherwise
 */
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

/**
 * @brief       Method that counts how many successes foo has had with strs
 * @param[in]   foo: Function that checks the strings for a pattern
 * @param[in]   strs: Vector os strings 
 * @return      Number of times a string in strs was matches in foo
 */
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

/**
 * @brief       Class to Check strings (Bonus)
 *              It was implemented to showcase the bind of a class member to a 
 *              std::function instead of using a lambda directly.
 */
class StringChecker
{
public:
    /**
     * @brief       Method to use instead of the lambda function on the 
     *              ContainsTheString test. 
     * @param[in]   teststr: String to test
     * @return      true if teststr equals "two, false otherwise 
     */
    bool CheckIfContainsTWO(const std::string& teststr)
    {
        return teststr == "two";
    }

private:
};

/**
 * @brief       Check if a variable is zero (default behavior will work on int numbers) - (Bonus)
 * @param[in]   var: Variable to be checked 
 * @return      true if value is zero
 * @warning     Since I didn't create all specializations some types are bound to fail
 */
template <typename T>
bool CheckIFZero(T var)
{
    return var == 0;
}
/**
 * @brief       Variadic template function to check if multiple variables are zeroed (Bonus)
 * @param[in]   var: first variable
 * @param[in]   vars: remaining variables
 * @return      true if all arguments are zero
 */
template <typename T, typename... types>
bool CheckIFZero(T var, types... vars)
{
    return CheckIFZero(var) && CheckIFZero(vars...);
}

/**
 * @brief       Specialization to check if float is considered zero (Bonus)
 * @param[in]   var: float variable
 * @return      true if float number is rounded up to zero
 */
template <>
bool CheckIFZero<float>(float var)
{
    static constexpr float FLOAT_ZERO_THREASHOLD = 0.09f;
    return (var < FLOAT_ZERO_THREASHOLD) && (var > -FLOAT_ZERO_THREASHOLD);
}
/**
 * @brief       Specialization to check if double is considered zero (Bonus)
 * @param[in]   var: double variable
 * @return      true if double number is rounded up to zero
 */
template <>
bool CheckIFZero<double>(double var)
{
    static constexpr double DOUBLE_ZERO_THREASHOLD = 0.009;

    return (var < DOUBLE_ZERO_THREASHOLD) && (var > -DOUBLE_ZERO_THREASHOLD);
}

/**
 * @brief       Specialization to check if string is considered zeroed (empty)(Bonus)
 * @param[in]   var: string variable
 * @return      true if string is cleared;
 */
template <>
bool CheckIFZero<std::string>(string var)
{
    return var == "";
}
