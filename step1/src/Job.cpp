/*
 * Copyright (c) 2023, Eberty Alves
 */

#include "Job.h"

#include <iostream>

Job::Job(const std::string &name, const std::string &description, unsigned int hoursRequired)
    : m_name(name), m_description(description), m_hoursRequired(hoursRequired)
{
}

Job::~Job()
{
}

std::string Job::GetName() const
{
    return m_name;
}

void Job::SetName(const std::string &name)
{
    m_name = name;
}

std::string Job::GetDescription() const
{
    return m_description;
}

void Job::SetDescription(const std::string &description)
{
    m_description = description;
}

unsigned int Job::GetHoursRequired() const
{
    return m_hoursRequired;
}

void Job::SetHoursRequired(unsigned int hoursRequired)
{
    m_hoursRequired = hoursRequired;
}

void Job::DoWork()
{
    std::cout << "My work involves " << m_description << std::endl;
}
