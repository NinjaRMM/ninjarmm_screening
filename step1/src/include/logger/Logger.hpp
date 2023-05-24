#pragma once

#include <iostream>
#include <string>

namespace logger {

class Logger {
public:
    static void Log(const std::string& message) {
        std::cout << message;
    }
};

} // namespace logger
