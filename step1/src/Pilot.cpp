/*
 * Copyright (c) 2023, Eberty Alves
 */

#include "Pilot.h"

#include <iostream>

Pilot::Pilot(const std::string &name,
             const std::string &description,
             unsigned int hoursRequired,
             const std::string &aircraftType)
    : Job(name, description, hoursRequired), m_aircraftType(aircraftType)
{
}

Pilot::~Pilot()
{
}

std::string Pilot::GetAircraftType() const
{
    return m_aircraftType;
}

void Pilot::SetAircraftType(const std::string &aircraftType)
{
    m_aircraftType = aircraftType;
}

void Pilot::DoWork()
{
    Job::DoWork();
    std::cout << "I fly on " << m_aircraftType << std::endl;
}
