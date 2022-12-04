#include "job.h"
#include <iostream>

std::string Job::Name() {
    return name;
}

std::string Job::Description() {
    return description;
}

int Job::Hours() {
    return hours;
}

void Job::DoWork() {
    std::cout << "My work involves ";
    std::cout << Description();
    std::cout << std::endl;
}
