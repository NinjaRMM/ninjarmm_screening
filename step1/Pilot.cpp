#include "Pilot.hpp"

#include <iostream>

Pilot::Pilot() :
	name_ ("Pilot"),
	description_ ("Operating and controlling aircraft along planned routes and during takeoffs and landings.\n" \
                  "Monitoring engines, fuel consumption, and other aircraft systems during flight.\n" \
				  "Responding to changing conditions, such as weather events and emergencies (for example, a mechanical malfunction)"), //taken from www.bls.gov
	hours_ (20)
{
}

std::string Pilot::GetName()
{
	return name_;
}

std::string Pilot::GetDescription()
{
	return description_;
}

uint32_t Pilot::GetHours()
{
	return hours_;
}

void Pilot::DoWork()
{
	std::cout << "My work involves " << GetDescription() << std::endl;;
}
