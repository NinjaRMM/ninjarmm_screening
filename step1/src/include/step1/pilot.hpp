#include <step1/job.hpp>

#ifndef PILOT_HPP
#define PILOT_HPP

class Pilot : public Job
{
public:
    Pilot(std::string employee_name);
    ~Pilot(void);

    void DescribeJob(void) const override;
    void DoWork(void) const override;
    void ImproveSkills(void) const override;
};

#endif // PILOT_HPP
