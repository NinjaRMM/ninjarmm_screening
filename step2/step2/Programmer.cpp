#include "Programmer.h"

std::string Programmer::GetName() const
{
    return std::string("Programmer");
}

std::string Programmer::GetDescription() const
{
    return std::string("converting coffee into code");
}

int Programmer::GetRequiredHours() const
{
    return 40;
}
