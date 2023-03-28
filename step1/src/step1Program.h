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

using namespace std;

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
    virtual void     DoWork(void)         = 0;

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
        return "I program things";
    }
    uint32_t GetReqHours(void) override
    {
        return 30;
    }
    void DoWork(void) override
    {
        printf("My work involves Programming\n");
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
        return "I pilot things";
    }
    uint32_t GetReqHours(void) override
    {
        return 40;
    }
    void DoWork(void) override
    {
        printf("My work involves Piloting\n");
    }

private:
};

template <typename T>
void IsInBounds(T val, T low, T high)
{
    cout << "Requested value: " << val << endl;
    cout << "Begin: " << low << endl;
    cout << "End: " << high << endl;
}
