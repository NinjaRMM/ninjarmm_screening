#include <iostream>
#include <string>

#pragma once

/**
@class Job
@brief Interface that represents a job.
The Job class provides an interface for jobs that can be performed.
It defines pure virtual methods for getting the name, description,
hours required, and performing the work.
*/
class Job {
public:
    /**
    @brief Get the name of the job.
    @return The name of the job as a string.
    */
    virtual std::string getName() const = 0;

    /**
    @brief Get the description of the job.
    @return The description of the job as a string.
    */
    virtual std::string getDescription() const = 0;

    /**
    @brief Get the number of hours required by the job.
    @return The number of hours required as an integer.
    */
    virtual int getHoursRequired() const = 0;

    /**
    @brief Perform the work associated with the job.
    This method should be implemented by derived classes to perform
    the specific work related to the job.
    It does not return any value.
    */
    virtual void doWork() const = 0;
};
