#pragma once

#include <string>

using namespace std;

namespace ninjarmm_screening_step1
{
/* Abstract class that represents a job */
class Job
{
public:
    virtual ~Job(){}
    virtual void DoOnboarding() noexcept = 0;
    virtual void DoWork() noexcept = 0;
    virtual bool CanWorkFromHome() const noexcept  = 0;
    virtual void Promote() noexcept = 0;
    virtual string GetName() const noexcept = 0;
};
}