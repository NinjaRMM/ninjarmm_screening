#include "test.hpp"

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

void printJobStatus(Job::Status status) {
  switch (status) {
  case Job::Status::kProgrammerJobCreated:
    std::cout << "Programmer Job Created" << std::endl;
    break;

  case Job::Status::kProgrammerJobStarted:
    std::cout << "Programmer Job Started" << std::endl;
    break;

  case Job::Status::kProgrammerJobStopped:
    std::cout << "Programmer Job Stopped" << std::endl;
    break;

  case Job::Status::kPilotJobCreated:
    std::cout << "Pilot Job Created" << std::endl;
    break;

  case Job::Status::kPilotJobStarted:
    std::cout << "Pilot Job Started" << std::endl;
    break;

  case Job::Status::kPilotJobStopped:
    std::cout << "Pilot Job Stopped" << std::endl;
    break;

  default:
    std::cout << "Unknown status" << std::endl;
    break;
  }
}

int main() {
  auto programmer = std::make_shared<Programmer>();
  auto pilot      = std::make_shared<Pilot>();
  auto jobs       = std::vector<std::shared_ptr<Job>> {
    programmer,
    pilot
  };

  for (const auto& job : jobs) {
    job->create();
    printJobStatus(job->status());

    job->start ();
    printJobStatus(job->status());

    job->stop  ();
    printJobStatus(job->status());
  }

  return 0;
}
