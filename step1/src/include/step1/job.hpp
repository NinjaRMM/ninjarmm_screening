#include <string>

#ifndef JOB_HPP
#define JOB_HPP

class Job
{
private:
    std::string employee_name_;

public:
    Job(std::string employee_name);
    virtual ~Job(void);

    virtual void DescribeJob(void) const = 0;
    virtual void DoWork(void) const = 0;
    virtual void ImproveSkills(void) const = 0;
    void SayName(void) const;
};

#endif // JOB_HPP
