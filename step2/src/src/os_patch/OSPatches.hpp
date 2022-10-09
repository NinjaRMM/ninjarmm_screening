#pragma once

#include <list>
#include <string>


class OSPatches {
public:
    OSPatches() = default;

    std::list<std::string> get() const;
};
