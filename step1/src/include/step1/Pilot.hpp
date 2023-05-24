#pragma once

#include "step1/Job.hpp"

namespace step1 {

class Pilot : public Job<> {
public:
    std::string GetName() const override;

    std::string GetDescription() const override;

    unsigned int GetRequiredHours() const override;
};

} // namespace step1
