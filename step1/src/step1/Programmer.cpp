#include "step1/Programmer.hpp"

namespace step1 {

std::string Programmer::GetName() const {
    return "Programmer";
}

std::string Programmer::GetDescription() const {
    return "writing code";
}

unsigned int Programmer::GetRequiredHours() const {
    return 8;
}

} // namespace step1

