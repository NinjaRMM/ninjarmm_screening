/*
 * Copyright (c) 2023, Eberty Alves
 */

#ifndef PROGRAMMER_H
#define PROGRAMMER_H

#include "Job.h"

#include <string>

/**
 * @brief A class that represents a programmer job.
 *
 * The Programmer class is a subclass of the Job class and provides additional functionalities specific to the job of
 * being a programmer, such as the programming language.
 */
class Programmer final : public Job
{
public:
    /**
     * @brief Constructor for the Programmer class.
     *
     * @param name The name of the job.
     * @param description The description of the job.
     * @param hoursRequired The number of hours required by the job.
     * @param programmingLanguage The programming language used in the job.
     */
    Programmer(const std::string &name,
               const std::string &description,
               unsigned int hoursRequired,
               const std::string &programmingLanguage);

    /**
     * @brief Destructor for the Programmer class.
     */
    ~Programmer();

    /**
     * @brief Get the programming language used in the job.
     *
     * @return The programming language used in the job.
     */
    std::string GetProgrammingLanguage() const;

    /**
     * @brief Set the programming language used in the job.
     *
     * @param programmingLanguage The new programming language used in the job.
     */
    void SetProgrammingLanguage(const std::string &programmingLanguage);

    /**
     * @brief Prints out a message describing the work of a programmer.
     *
     * Overrides the DoWork() function of the base class, printing out a message describing the work of a programmer,
     * including your programming language.
     */
    void DoWork() override;

private:
    std::string m_programmingLanguage = ""; /**< The programming language used by the programmer. */
};

#endif // PROGRAMMER_H
