#pragma once

#include <chrono>
#include <iostream>
#include <string>

namespace ninja {

class Job {
public:
  virtual std::string get_jobs_name() const = 0;
  virtual std::string get_jobs_description() const = 0;
  virtual std::chrono::hours get_jobs_duration() const = 0;
  virtual void DoWork() const {
    std::cout << "My work involves " << get_jobs_description() << std::endl;
  }
  virtual ~Job() = default;
};

} // namespace ninja
