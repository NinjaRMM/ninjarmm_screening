#include "Pilot.h"

std::uint32_t Pilot::GetFlightTime() {
    return this->flightTime;
}

std::string Pilot::MinimumRequirements() {
    return "The pilot must have at least " + std::to_string(GetFlightTime()) + " hours of flight time.";
}