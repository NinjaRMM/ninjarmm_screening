#ifndef STEP1_JOB_HPP
#define STEP1_JOB_HPP

#include <string>

class Job
{
public:
	virtual std::string GetName() = 0;
	virtual std::string GetDescription() = 0;
	virtual uint32_t GetHours() = 0;
	virtual void DoWork() = 0;
};

#endif

