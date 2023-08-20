#include <iostream>
#include <memory>
#include <vector>

#include "Jobs.hpp"

int main() {

  std::vector<std::unique_ptr<Job>> allJobs;
  allJobs.emplace_back(std::make_unique<Pilot>());
  allJobs.emplace_back(std::make_unique<Programmer>(true));

  // No need to use a shared pointer, in this specific case it should be safe 
  // to get the reference for the unique pointer instead.
  for (std::unique_ptr<Job> &job : allJobs) {
    std::cout << "Current stress level: " << job->currentStressLevel() << "\n";
    job->makeDecision(true);
    std::cout << "Need vacation: " << job->needVacation() << "\n";
    std::cout << job->flexOnParty() << "\n\n";
  }

  return 0;
}
