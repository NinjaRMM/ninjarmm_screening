#pragma once
#include <string>

class Job {

public:
	virtual std::string Name() = 0;
	virtual std::string Description() = 0;
	virtual int Hours() = 0;
	virtual void DoWork() = 0;

protected:
	std::string name = "Job";
	std::string description = "Description";
	int hours = 0;
};