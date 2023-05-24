#pragma once

#include "logger/Logger.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace step1 {

template<class Logger = logger::Logger>
class Job {
public:
    virtual std::string GetName() const = 0;

    virtual std::string GetDescription() const = 0;

    virtual unsigned int GetRequiredHours() const = 0;

    void DoWork() const {
        std::stringstream ss;
        ss << "My work involves " << this->GetDescription() << std::endl;
        Logger::Log(ss.str());
    }
};

extern template class Job<logger::Logger>;
} // namespace step1
