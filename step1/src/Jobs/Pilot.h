#pragma once


#include "Job.h"

namespace ninjarmm_screening_step1
{
class Pilot: public Job
{
public:
    void DoOnboarding() noexcept override;
    void DoWork() noexcept override;
    bool CanWorkFromHome() const noexcept override;
    void Promote() noexcept override;
    string GetName() const noexcept override;
};
}