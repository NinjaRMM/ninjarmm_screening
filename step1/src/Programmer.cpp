/*
 * Copyright (c) 2023, Eberty Alves
 */

#include "Programmer.h"

#include <iostream>

Programmer::Programmer(const std::string &name,
                       const std::string &description,
                       unsigned int hoursRequired,
                       const std::string &programmingLanguage)
    : Job(name, description, hoursRequired), m_programmingLanguage(programmingLanguage)
{
}

Programmer::~Programmer()
{
}

std::string Programmer::GetProgrammingLanguage() const
{
    return m_programmingLanguage;
}

void Programmer::SetProgrammingLanguage(const std::string &programmingLanguage)
{
    m_programmingLanguage = programmingLanguage;
}

void Programmer::DoWork()
{
    Job::DoWork();
    std::cout << "I program in " << m_programmingLanguage << std::endl;
}
