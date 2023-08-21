#include <iostream>
#include <memory>
#include <vector>

#include "Jobs.hpp"
#include "TemplatedBounds.hpp"
#include "StringCount.hpp"

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

  uint32_t httpResonseCode = 420;
  std::cout << "Is within bounds: " << IsInBounds<uint32_t>(httpResonseCode, 500, 599) << "\n\n";

  auto theStrings = std::vector<std::string> { "one", "two",  "test"};
  auto count = ContainsTheString([](const std::string& tested) { return tested == "test";}, theStrings);
  std::cout << "Number of occurences of string: " << count << "\n";

  return 0;
}
