
#pragma once

#include <string>

class Job
{
public:
    Job() = default;
    virtual ~Job() = default;

    virtual int32_t Action1() { return 0; };
    virtual bool Action2() { return false; };
    virtual std::string Action3() { return ""; };
    virtual float Action4() { return 0.0f; };
};
