#include <iostream>
#include <string>

#include "common_job.h"

#pragma once


/**
@class Pilot
@brief A specific type of job representing a pilot.
The Pilot class represents a job of being a pilot. It inherits from the
CommonJob class, which itself is a concrete common implementation of the Job interface.
The Pilot class adds additional functionality specific to being a pilot.
*/
class Pilot : public CommonJob {
public:
    /**
    @brief Default constructor.
    Constructs a Pilot object with default values for its member variables.
    */
    Pilot();

    /**
    @brief Parameterized constructor.
    Constructs a Pilot object with the specified name, description, and hours required.
    @param name The name of the job.
    @param description The description of the job.
    @param hoursRequired The number of hours required for the job.
    */
    Pilot(const std::string& name, const std::string& description, const uint16_t& hoursRequired);

    /**
    @brief Get a string representation of the Pilot object.
    Returns a string representation of the Pilot object, including its name,
    description, and hours required.
    @return The string representation of the Pilot object.
    */
    std::string toString() const;

private:
    static const std::string m_defaultName;
    static const std::string m_defaultDescription;
    static const std::uint16_t m_defaultHoursRequired;
};
