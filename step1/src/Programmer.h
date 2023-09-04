#pragma once
#include "Job.h"

class Programmer: public Job {
private:
    std::string stack;

public:
    Programmer(std::string_view _description, std::string_view _stack)
    : stack(_stack), Job("Programmer", _description, 40) {}
    
    Programmer(Programmer &other) : Programmer(other.GetDescription(), other.GetStack()) {}

    ~Programmer() = default;
    
    std::string GetStack();
    std::string MinimumRequirements() override;

};