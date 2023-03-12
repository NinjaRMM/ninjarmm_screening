#include "Programmer.h"

namespace ninja {

Programmer::Programmer(std::chrono::hours job_duration)
    : m_job_duration(std::move(job_duration)) {}

std::string Programmer::get_jobs_name() const { return "Programmer"; }

std::string Programmer::get_jobs_description() const {
  return "coding programms";
}

std::chrono::hours Programmer::get_jobs_duration() const {
  return m_job_duration;
}

} // namespace ninja
