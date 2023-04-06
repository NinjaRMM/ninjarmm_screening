/**
 * @file Pilot.cpp
 *
 * @brief Implementation of the Pilot class.
 *
 * @author   gustavo.garcia
 * @date     Apr 02, 2023
 * @revision 1
 *
 */

/* INCLUDES ******************************************************************/

#include <iostream>
#include "Pilot.hpp"

/* CLASS IMPLEMENTATION ******************************************************/

std::string Pilot::GetName() const {
    return "Pilot";
}

std::string Pilot::GetDescription() const {
    return "Driving racing cars.";
}

int Pilot::GetHoursRequired() const {
    return 40;
}

void Pilot::DoWork() const {
    std::cout << "My work involves " << GetDescription() << std::endl;
}
