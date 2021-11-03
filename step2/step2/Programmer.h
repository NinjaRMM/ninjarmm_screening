#include "Job.h"

class Programmer : public Job
{
public:
    std::string GetName() const override;
    std::string GetDescription() const override;
    int GetRequiredHours() const override;
};