#pragma once

#include <string>


class SystemInfo {
public:
    SystemInfo() = default;

    std::string platform() const;
    std::string cpu() const;
    std::string memory() const;
    std::string disk() const;
};
