#include "Job.h"
#include <iostream>

void Job::DoWork() {
	std::cout << "My work involves " + description << std::endl;
}
std::string Job::Name() {
	return name;
}

std::string Job::Description() {
	return description;
}

int Job::Hours() {
	return hours;
}