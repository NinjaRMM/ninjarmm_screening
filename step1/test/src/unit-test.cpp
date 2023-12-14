#include "test.hpp"

#include "doctest.h"

TEST_CASE("Status is correctly set for Programmer job class") {
  Programmer job;

  CHECK_EQ(job.status(), Job::Status::kUninitialized);

  job.create();
  CHECK_EQ(job.status(), Job::Status::kProgrammerJobCreated);

  job.start();
  CHECK_EQ(job.status(), Job::Status::kProgrammerJobStarted);

  job.stop();
  CHECK_EQ(job.status(), Job::Status::kProgrammerJobStopped);
}

TEST_CASE("Status is correctly set for Pilot job class") {
  Pilot job;

  CHECK_EQ(job.status(), Job::Status::kUninitialized);

  job.create();
  CHECK_EQ(job.status(), Job::Status::kPilotJobCreated);

  job.start();
  CHECK_EQ(job.status(), Job::Status::kPilotJobStarted);

  job.stop();
  CHECK_EQ(job.status(), Job::Status::kPilotJobStopped);
}

TEST_CASE("containsTheString returns the number of occurrences satisfying a predicate") {
  auto theStrings   = std::vector<std::string> { "one", "two", "test", "repeated", "repeated" };
  auto stringToTest = "";
  auto testerFn     = [&](const std::string& tested) { return tested == stringToTest; };

  CHECK_EQ(containsTheString(testerFn, theStrings), 0);

  stringToTest = "test";
  CHECK_EQ(containsTheString(testerFn, theStrings), 1);

  stringToTest = "repeated";
  CHECK_EQ(containsTheString(testerFn, theStrings), 2);
}

TEST_CASE("IsInBound returns true for a value within a range") {
  CHECK_EQ(IsInBound<uint32_t>(100, 100, 599), true);
  CHECK_EQ(IsInBound<uint32_t>(300, 100, 599), true);
  CHECK_EQ(IsInBound<uint32_t>(599, 100, 599), true);
}

TEST_CASE("IsInBound returns false for a value not within a range") {
  CHECK_EQ(IsInBound<uint32_t>( 99, 100, 599), false);
  CHECK_EQ(IsInBound<uint32_t>(600, 100, 599), false);
}

TEST_CASE("IsValidHttpResponseCode returns true if all the values are valid HTTP status codes") {
  CHECK_EQ(IsValidHttpResponseCode(100, 200, 300, 400, 500, 599), true);
}

TEST_CASE("IsValidHttpResponseCode returns false if any of the values are NOT valid HTTP status codes") {
  CHECK_EQ(IsValidHttpResponseCode( 99, 100, 200, 300, 400, 500, 599), false);
  CHECK_EQ(IsValidHttpResponseCode(100, 200, 300, 400, 500, 599, 600), false);
  CHECK_EQ(IsValidHttpResponseCode(100, 200, 300, 400, 700, 500, 599), false);
}
