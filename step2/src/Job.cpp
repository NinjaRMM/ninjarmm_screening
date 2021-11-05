#include "Job.h"

std::string Job::DoWork() const
{
    return std::string("My work involves ") + GetDescription();
}