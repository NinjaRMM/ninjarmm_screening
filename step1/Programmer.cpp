#include "Programmer.hpp"

#include <iostream>

Programmer::Programmer() :
	name_("Programmer"),
	description_("Writing, modifying, and testing code and scripts that allow computer software and applications to function properly.\n" \
				 "Turning the designs created by software developers and engineers into instructions that a computer can follow."), // taken from www.bls.gov
	hours_(40)
{
}

std::string Programmer::GetName()
{
	return name_;
}

std::string Programmer::GetDescription()
{
	return description_;
}

uint32_t Programmer::GetHours()
{
	return hours_;
}

void Programmer::DoWork()
{
	std::cout << "My work involves " << GetDescription() << std::endl;;
}
