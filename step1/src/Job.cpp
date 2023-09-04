#include "Job.h"

std::string Job::GetName() {
    return this->name;
}

std::string Job::GetDescription() {
    return this->description;
}

std::uint8_t Job::GetWorkHours() {
    return this->workHours;
}
