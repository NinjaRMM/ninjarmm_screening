#pragma once

#include <iostream>

// a. Create a class Job

class job {
  // d. The job class should provide an interface that does at least 4 actions.
  //     i. Please decide what 4 actions you would like your job to do, with each
  //        action being represented as a function.
 public:
  void learn () const
  {
	std::cout << "learning about be committed" << std::endl;
  }

  virtual void work () const
  {
	std::cout << "do the work" << std::endl;
  }

  void rest () const
  {
  }

  void retire () const
  {
  }
};
