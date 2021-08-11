#pragma once
#include "Job.h"

class Pilot : public Job {

public:
	Pilot();

	std::string Name();
	std::string Description();
	int Hours();
	void DoWork();
};