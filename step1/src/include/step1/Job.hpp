#pragma once

#include "logger/Logger.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace step1 {

template<class Logger = logger::Logger>
class Job {
public:
    inline std::string GetName() const {
        return this->name_;
    }

    inline std::string GetDescription() const {
        return this->description_;
    }

    inline unsigned int GetRequiredHours() const {
        return this->requiredHours_;
    }

    Job(const std::string& name,
        const std::string& description,
        unsigned int requiredHours):
            name_(name),
            description_(description),
            requiredHours_(requiredHours) {}

    void DoWork() const {
        std::stringstream ss;
        ss << "My work involves " << this->GetDescription() << std::endl;
        Logger::Log(ss.str());
    }

private:
    std::string name_;
    std::string description_;
    unsigned int requiredHours_;
};

extern template class Job<logger::Logger>;
} // namespace step1
