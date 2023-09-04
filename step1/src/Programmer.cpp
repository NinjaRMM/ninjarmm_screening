#include "Programmer.h"

std::string Programmer::GetStack() {
    return this->stack;
}

std::string Programmer::MinimumRequirements() {
    return "The programmer must have at least: " + this->GetStack();
}