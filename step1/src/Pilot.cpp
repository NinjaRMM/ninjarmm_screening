#include "Pilot.h"

namespace ninja {

Pilot::Pilot(std::chrono::hours job_duration)
    : m_job_duration(std::move(job_duration)) {}

std::string Pilot::get_jobs_name() const { return "Pilot"; }
std::string Pilot::get_jobs_description() const { return "flying planes"; }
std::chrono::hours Pilot::get_jobs_duration() const { return m_job_duration; }

} // namespace ninja
