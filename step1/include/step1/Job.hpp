#pragma once

#include "logger/Logger.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace step1 {

template<class Logger = logger::Logger>
class Job {
public:
    std::string GetName() { return name_; }

    virtual std::string GetDescription() const = 0;

    unsigned GetRequiredHours() const { return requiredHours_; }

    void DoWork() const {
        std::stringstream ss;
        ss << "My work involves " << this->GetDescription() << std::endl;
        Logger::Log(ss.str());
    }

    Job(const std::string &name,
        unsigned requiredHours)
        : name_(name),
          requiredHours_(requiredHours) {}

  protected:
    std::string name_;
    unsigned requiredHours_;
};

extern template class Job<logger::Logger>;
} // namespace step1
