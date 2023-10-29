#include <iostream>
#include <step1/pilot.hpp>

Pilot::Pilot(std::string employee_name) : Job(employee_name) {}

Pilot::~Pilot(void) {}

void Pilot::DescribeJob(void) const
{
    std::cout << "I am a Pilot!\n";
}

void Pilot::DoWork(void) const
{
    std::cout << "Piloting right now, boss.\n";
}

void Pilot::ImproveSkills(void) const
{
    std::cout << "Using the piloting simulator to improve my skills.\n";
}
