#pragma once
#include "Job.h"

class Pilot: public Job {
private:
    std::uint32_t flightTime;

public:
    Pilot(const std::string_view _description, std::uint32_t _flightTime)
    : Job("Pilot", _description, 44U), flightTime(_flightTime) {}
    ~Pilot() = default;

    std::uint32_t GetFlightTime();
    std::string MinimumRequirements() override;

};