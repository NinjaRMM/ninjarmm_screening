#include <string>
#ifndef JOB_H
#define JOB_H

class Job
{
public:
    virtual std::string GetName() const = 0;
    virtual std::string GetDescription() const = 0;
    virtual int GetRequiredHours() const = 0;
    std::string DoWork() const;

    virtual ~Job() {};
};

#endif // !JOB_H
