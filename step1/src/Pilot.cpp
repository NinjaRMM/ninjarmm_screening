/*
 * Pilot.cpp
 *
 *  Created on: Dec 21, 2023
 *      Author: alex
 */

#include "Pilot.h"

namespace ninjans
{

Pilot::Pilot()
{
	m_hasUniform = true;
	m_isRemoteWorker = false;
	m_title = "Co-pilot";

}

Pilot::~Pilot()
{
//	std::cout << "Pilot DTOR" <<std::endl;
}

std::string Pilot::Work() const
{
	return "I fly planes.";
}

bool Pilot::Fly() const
{
	return true;
}

} /* namespace ninjans */
