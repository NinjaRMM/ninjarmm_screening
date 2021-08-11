#include "Pilot.h"
#include <iostream>

Pilot::Pilot() {
	name = "Pilot";
	description = "flying planes";
	hours = 12;
}

void Pilot::DoWork() {
	std::cout << "My work involves " + description << std::endl;
}

std::string Pilot::Name() {
	return name;
}

std::string Pilot::Description() {
	return description;
}

int Pilot::Hours() {
	return hours;
}
