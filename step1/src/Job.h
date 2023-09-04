#pragma once

#include <string>

class Job
{
private:
    std::string name;
    std::string description;
    std::uint8_t workHours;

public:
    Job(const std::string_view _name,
        const std::string_view _description,
        std::uint8_t _workHours)
        : name(_name), description(_description), workHours(_workHours) {}
    virtual ~Job() = default;

    std::string GetName();
    std::string GetDescription();
    std::uint8_t GetWorkHours();

    virtual std::string MinimumRequirements() = 0;
};