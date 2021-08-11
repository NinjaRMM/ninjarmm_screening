#pragma once
#include "Job.h"

class Programmer : public Job {

public:
	Programmer();

	std::string Name();
	std::string Description();
	int Hours();
	void DoWork();
};