#include <iostream>
#include <string>

#include "job.h"

#pragma once

/**
@class CommonJob
@brief A concrete implementation of the Job interface.
The CommonJob class represents a common type of job that can be performed.
It inherits from the interface Job and provides implementations
for the virtual methods defined in the base class.
*/
class CommonJob : public Job {
public:
    /**
    @brief Get the name of the job.
    @return The name of the job as a string.
    */
    std::string getName() const override;

    /**
    @brief Get the description of the job.
    @return The description of the job as a string.
    */
    std::string getDescription() const override;

    /**
    @brief Get the number of hours required by the job.
    @return The number of hours required as an integer.
    */
    int getHoursRequired() const override;

    /**
    @brief Perform the work associated with the job.
    This method performs a basic common work.
    It does not return any value.
    */
    void doWork() const override;

protected:
    /**
    @brief Parameterized constructor.
    Constructs a CommonJob object with the specified name, description, and hours required.
    @param name The name of the job.
    @param description The description of the job.
    @param hoursRequired The number of hours required for the job.
    */
    CommonJob(const std::string& name, const std::string& description, const uint16_t& hoursRequired);

protected:
    std::string m_name;
    std::string m_description;
    std::uint16_t m_hoursRequired;
};
