#include <iostream>
#include <step1/programmer.hpp>

Programmer::Programmer(std::string employee_name) : Job(employee_name) {}

Programmer::~Programmer(void) {}

void Programmer::DescribeJob(void) const
{
    std::cout << "I am a Programmer!\n";
}

void Programmer::DoWork(void) const
{
    std::cout << "Programming right now, boss.\n";
}

void Programmer::ImproveSkills(void) const
{
    std::cout << "Doing code challenges to improve my skills.\n";
}
