/*
 * Copyright (c) 2023, Eberty Alves
 */

#ifndef JOB_H
#define JOB_H

#include <string>

/**
 * @brief A class representing a job.
 *
 * This class provides an interface for a job, which includes a name, a description, and the number of hours required
 * by the job.
 */
class Job
{
public:
    /**
     * @brief Constructor for the Job class.
     *
     * @param name The name of the job.
     * @param description The description of the job.
     * @param hoursRequired The number of hours required by the job.
     */
    Job(const std::string &name, const std::string &description, unsigned int hoursRequired);

    /**
     * @brief Virtual destructor for the Job class.
     */
    virtual ~Job();

    /**
     * @brief Get the name of the job.
     *
     * @return The name of the job.
     */
    std::string GetName() const;

    /**
     * @brief Set the name of the job.
     *
     * @param name The new name of the job.
     */
    void SetName(const std::string &name);

    /**
     * @brief Get the description of the job.
     *
     * @return The description of the job.
     */
    std::string GetDescription() const;

    /**
     * @brief Set the description of the job.
     *
     * @param description The new description of the job.
     */
    void SetDescription(const std::string &description);

    /**
     * @brief Get the number of hours required by the job.
     *
     * @return The number of hours required by the job.
     */
    unsigned int GetHoursRequired() const;

    /**
     * @brief Set the number of hours required by the job.
     *
     * @param hoursRequired The new number of hours required by the job.
     */
    void SetHoursRequired(unsigned int hoursRequired);

    /**
     * @brief Performs the work associated with the job.
     *
     * This is a virtual function that can be overridden by derived classes to perform work specific to the type of job.
     *
     * The default implementation prints out "My work involves " followed by the job's description.
     */
    virtual void DoWork();

private:
    std::string m_name = "";          /**< The name of the job. */
    std::string m_description = "";   /**< The description of the job. */
    unsigned int m_hoursRequired = 0; /**< The number of hours required by the job. */
};

#endif // JOB_H
