#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

template <typename NAME, typename DESCRIPTION, typename HOURS> class Job_t {
public:
  using Name = NAME;
  using Description = DESCRIPTION;
  using Hours = HOURS;

  using Ptr = std::shared_ptr<Job_t<NAME, DESCRIPTION, HOURS>>;

public:
  Job_t() = delete;
  Job_t(const Job_t &) = delete;
  Job_t(Job_t &&) = delete;

  template <typename JOB> static Ptr create(Hours pHours) {
    return std::make_shared<JOB>(pHours);
  }

  virtual Name name() const { return mName; }
  virtual Description description() const { return mDescription; }
  virtual Hours hours() const { return mHours; }

  void DoWork() const {
    std::cout << "My work involves " << mDescription << '\n';
  }

  Job_t &operator=(const Job_t &) = delete;
  Job_t &operator=(Job_t &&) = delete;

protected:
  Job_t(Name &&pName, Description &&pDescription, Hours pHours)
      : mName(std::move(pName)), mDescription(std::move(pDescription)),
        mHours(pHours) {}

protected:
  Name mName;
  Description mDescription;
  Hours mHours;
};

template <typename NAME, typename DESCRIPTION, typename HOURS>
class Programmer_t : public Job_t<NAME, DESCRIPTION, HOURS> {
public:
  Programmer_t(HOURS pHours)
      : Job_t<NAME, DESCRIPTION, HOURS>("Programmer",
                                        "Writes ninja level programs", pHours) {
  }
};

template <typename NAME, typename DESCRIPTION, typename HOURS>
class Pilot_t : public Job_t<NAME, DESCRIPTION, HOURS> {
public:
  Pilot_t(HOURS pHours)
      : Job_t<NAME, DESCRIPTION, HOURS>("Pilot", "Flies all over the world",
                                        pHours) {}
};

template <typename NAME, typename DESCRIPTION, typename HOURS>
using Jobs_t = std::vector<typename Job_t<NAME, DESCRIPTION, HOURS>::Ptr>;

template <typename JOBS, typename JOB>
void add_job(JOBS &pJobs, typename JOB::Hours hours) {
  pJobs.push_back(JOB::template create<JOB>(hours));
}

template <typename JOBS, typename JOB_PTR>
void traverse_jobs(JOBS &pJobs, std::function<void(JOB_PTR)> visit) {

  for (JOB_PTR jobPtr : pJobs) {
    visit(jobPtr);
  }
}

using Job = Job_t<std::string, std::string, uint16_t>;
using Programmer = Programmer_t<Job::Name, Job::Description, Job::Hours>;
using Pilot = Pilot_t<Job::Name, Job::Description, Job::Hours>;
using Jobs = Jobs_t<Job::Name, Job::Description, Job::Hours>;
using JobPtr = typename Jobs::value_type;

int main() {
  Jobs jobs;
  add_job<Jobs, Programmer>(jobs, Programmer::Hours{1874});
  add_job<Jobs, Programmer>(jobs, Programmer::Hours{48});
  add_job<Jobs, Pilot>(jobs, Pilot::Hours{193});

  traverse_jobs<Jobs, JobPtr>(jobs, [](JobPtr jobPtr) -> void {
    std::cout << "Name = " << jobPtr->name() << ", description = '"
              << jobPtr->description() << "', hours = " << jobPtr->hours()
              << '\n';
  });

  return 0;
}
