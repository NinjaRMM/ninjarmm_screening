#include <iostream>
#include <string>

#include "common_job.h"

#pragma once

/**
@class Programmer
@brief A specific type of job representing a programmer.
The Programmer class represents a job of being a programmer. It inherits from the
CommonJob class, which itself is a concrete common implementation of the Job interface.
*/
class Programmer : public CommonJob {
public:
    /**
    @brief Default constructor.
    Constructs a Programmer object with default values for its member variables.
    */
    Programmer();

    /**
    @brief Parameterized constructor.
    Constructs a Programmer object with the specified name, description, and hours required.
    @param name The name of the job.
    @param description The description of the job.
    @param hoursRequired The number of hours required for the job.
    */
    Programmer(const std::string& name, const std::string& description, const uint16_t& hoursRequired);

private:
    static const std::string m_defaultName;
    static const std::string m_defaultDescription;
    static const std::uint16_t m_defaultHoursRequired;
};
