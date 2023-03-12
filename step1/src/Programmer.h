#pragma once

#include "Job.h"

namespace ninja {

class Programmer : public Job {
public:
  Programmer(std::chrono::hours job_duration);
  std::string get_jobs_name() const override;
  std::string get_jobs_description() const override;
  std::chrono::hours get_jobs_duration() const override;
  ~Programmer() = default;

private:
  // Let's assume that each instance of this class can have
  // it's own job duration.
  std::chrono::hours m_job_duration;
};

} // namespace ninja
