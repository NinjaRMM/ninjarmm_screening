#include "Pilot.h"

std::string Pilot::GetName() const
{
    return std::string("Pilot");
}

std::string Pilot::GetDescription() const
{
    return std::string("flying air planes");
}

int Pilot::GetRequiredHours() const
{
    return 80;
}

