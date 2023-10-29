#include <step1/job.hpp>

#ifndef PROGRAMMER_HPP
#define PROGRAMMER_HPP

class Programmer : public Job
{
public:
    Programmer(std::string employee_name);
    ~Programmer(void);

    void DescribeJob(void) const override;
    void DoWork(void) const override;
    void ImproveSkills(void) const override;
};

#endif // PROGRAMMER_HPP
