#pragma once

#include "job.hpp"

#include <iostream>

// c. Create a subclass Pilot

class pilot : public job {
 public:
	void work () const override
	{
	  std::cout << "pilot!" << std::endl;
	}
};
