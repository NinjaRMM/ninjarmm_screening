#include "header.hpp"
#include <memory>
#include <array>
#include <iostream>
#include <vector>

void printArray() {
  std::cout << std::endl;
}

template <typename FirstElement>
void printArray(const FirstElement& firstElement) {
    std::cout << firstElement << std::endl;
}

template <typename FirstElement, typename... List>
void printArray(const FirstElement& firstElement, const List&... list) {
    std::cout << firstElement << ", ";
    printArray(list...);
}


int main() {
  constexpr uint8_t nbJobs = 2;
  std::array<std::unique_ptr<Job>, nbJobs> jobs {
    std::make_unique<Programmer>("John", "Doe"),
    std::make_unique<Pilot>()
  };

  for (const auto& job: jobs) {
    job->performTask();
    job->isEmployed();
    job->helpTeammate();
    job->attendMeeting();
  }

  std::cout << IsInBounds<uint32_t>(500, 0, 1000) << std::endl;
  std::cout << IsInBounds<uint32_t>(1000, 0, 500) << std::endl;
  std::cout << IsInBounds<uint32_t>(200, 0, 100) << std::endl;

  auto theStrings = std::vector<std::string> { "one", "two",  "test"};
  auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
  std::cout << "count: " << count << std::endl;
  
  printArray(1, 2, 3, 4, 5);

  return 0;
}