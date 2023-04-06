/**
 * @file Programmer.cpp
 *
 * @brief Implementation of the Programmer class.
 *
 * @author   gustavo.garcia
 * @date     Apr 02, 2023
 * @revision 1
 *
 */

/* INCLUDES ******************************************************************/

#include <iostream>
#include "Programmer.hpp"

/* CLASS IMPLEMENTATION ******************************************************/

std::string Programmer::GetName() const {
    return "Programmer";
}

std::string Programmer::GetDescription() const {
    return "Writing good quality code.";
}

int Programmer::GetHoursRequired() const {
    return 41;
}

void Programmer::DoWork() const {
    std::cout << "My work involves " << GetDescription() << std::endl;
}
