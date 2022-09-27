#pragma once
#include <string>

enum class PatchType
{
    Install = 1,
    Upgrade,
    Remove
};

struct PatchInfo
{
    std::string name;
    PatchType type;
    std::string type_description;
};


