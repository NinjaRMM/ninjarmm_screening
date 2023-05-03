/*
 * Copyright (c) 2023, Eberty Alves
 */

#ifndef PILOT_H
#define PILOT_H

#include "Job.h"

#include <string>

/**
 * @brief A class that represents a job position of a Pilot.
 *
 * The Pilot class is a subclass of the Job class and provides additional functionalities specific to the job of being
 * a pilot, such as aircraft type.
 */
class Pilot final : public Job
{
public:
    /**
     * @brief Constructor for the Pilot class.
     *
     * @param name The name of the job.
     * @param description The description of the job.
     * @param hoursRequired The number of hours required by the job.
     * @param aircraftType The type of aircraft the pilot is able to fly.
     */
    Pilot(const std::string &name,
          const std::string &description,
          unsigned int hoursRequired,
          const std::string &aircraftType);

    /**
     * @brief Destructor for the Pilot class.
     */
    ~Pilot();

    /**
     * @brief Get the type of aircraft the pilot is able to fly.
     *
     * @return The type of aircraft.
     */
    std::string GetAircraftType() const;

    /**
     * @brief Set the type of aircraft the pilot is able to fly.
     *
     * @param aircraftType The new type of aircraft.
     */
    void SetAircraftType(const std::string &aircraftType);

    /**
     * @brief Prints out a message describing the work of a pilot.
     *
     * Overrides the DoWork() function of the base class, printing out a message describing the work of a pilot,
     * including the aircraft type he operates.
     */
    void DoWork() override;

private:
    std::string m_aircraftType = ""; /**< The type of aircraft that the Pilot operates. */
};

#endif // PILOT_H
