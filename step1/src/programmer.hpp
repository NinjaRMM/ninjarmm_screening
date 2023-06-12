#pragma once

#include "job.hpp"

#include <iostream>

// b. Create a subclass Programmer

class programmer : public job {
 public:
  void work () const override
  {
	std::cout << "code!" << std::endl;
  }
};
