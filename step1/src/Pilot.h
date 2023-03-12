#pragma once

#include "Job.h"

namespace ninja {

class Pilot : public Job {
public:
  Pilot(std::chrono::hours job_duration);
  std::string get_jobs_name() const override;
  std::string get_jobs_description() const override;
  virtual std::chrono::hours get_jobs_duration() const override;
  ~Pilot() = default;

private:
  // Let's assume that each instance of this class can have
  // it's own job duration.
  std::chrono::hours m_job_duration;
};

} // namespace ninja
