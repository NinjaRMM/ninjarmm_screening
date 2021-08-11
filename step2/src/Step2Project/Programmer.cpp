#include "Programmer.h"
#include <iostream>

Programmer::Programmer() {
	name = "Programmer";
	description = "writing code";
	hours = 1;
}

void Programmer::DoWork() {
	std::cout << "My work involves " + description << std::endl;
}

std::string Programmer::Name() {
	return name;
}

std::string Programmer::Description() {
	return description;
}

int Programmer::Hours() {
	return hours;
}
