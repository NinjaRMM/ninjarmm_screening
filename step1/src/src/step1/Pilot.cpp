#include "step1/Pilot.hpp"

namespace step1 {

std::string Pilot::GetName() const {
    return "Pilot";
}

std::string Pilot::GetDescription() const {
    return "racing a car";
}

unsigned int Pilot::GetRequiredHours() const {
    return 12;
}

} // namespace step1
