#ifndef STEP1_JOB_HPP
#define STEP1_JOB_HPP

#include <string>

class Job
{
public: // Requirement a., d.
	virtual ~Job() = default;
	virtual std::string GetName() = 0; // requirement d.i
	virtual std::string GetDescription() = 0; // requirement d.ii
	virtual uint32_t GetHours() = 0; // requirement d.iii
	virtual void DoWork() = 0; // requirement d.iv
};

#endif

