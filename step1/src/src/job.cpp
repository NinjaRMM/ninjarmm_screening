#include <iostream>
#include <step1/job.hpp>

Job::Job(std::string employee_name) : employee_name_{employee_name} {}

Job::~Job(void) {}

void Job::SayName(void) const
{
    std::cout << "My name is " << employee_name_ << "!\n";
}
