#ifndef STEP1_PILOT_HPP
#define STEP1_PILOT_HPP
#include "Job.hpp"

class Pilot : public Job
{
public:
	Pilot();
	~Pilot() = default;
	virtual std::string GetName() override;
	virtual std::string GetDescription() override;
	virtual uint32_t GetHours() override;
	virtual void DoWork() override;
private:
	std::string name_;
	std::string description_;
	uint32_t hours_;
};

#endif