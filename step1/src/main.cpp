#include "Job.h"
#include "Pilot.h"
#include "Programmer.h"
#include "microtest.h"
#include <memory>
#include <vector>

// USAGE:
// to compile: g++ -std=c++20 *.cpp -o out
// to run ./out
// Formatted with clang-format -i *.h *.cpp

// minor remarks : 'auto count = ContainsTheString([](const std::string& tested)
// { return tested == “test” }, theStrings);' ; after == "test" is missing. “ -
// doesnt compile had to replace by regular qoutes "

namespace {

template <typename T>
bool IsInBounds(const T &value, const T &left_bound, const T &right_bound) {
  const auto result = left_bound <= value && value <= right_bound;
  std::string result_str = result ? "within bounds" : "out of bounds";
  std::cout << "The left bound is: " << left_bound << std::endl
            << "The right bound is: " << right_bound << std::endl
            << "The value is: " << value << std::endl
            << "Result: The value is " << result_str << std::endl;
  return result;
}

size_t
ContainsTheString(const std::function<bool(const std::string &)> &test_function,
                  std::vector<std::string> strings) {
  size_t count = 0;
  for (const auto &s : strings) {
    count = test_function(s) ? count + 1 : count;
  }
  return count;
}

} // namespace

using namespace ninja;
using namespace std::chrono_literals;

//// 1.k ////
TEST(Pilot_get_jobs_name) {
  Pilot p(5h);
  ASSERT_EQ("Pilot", p.get_jobs_name());
}

TEST(Pilot_get_jobs_description) {
  Pilot p(5h);
  ASSERT_EQ("flying planes", p.get_jobs_description());
}

TEST(Pilot_get_jobs_duration) {
  const std::chrono::hours duration_to_set = 5h;
  Pilot p(duration_to_set);
  ASSERT_TRUE(duration_to_set == p.get_jobs_duration());
}

TEST(Programmer_get_jobs_name) {
  Programmer p(5h);
  ASSERT_EQ("Programmer", p.get_jobs_name());
}

TEST(Programmer_get_jobs_description) {
  Programmer p(5h);
  ASSERT_EQ("coding programms", p.get_jobs_description());
}

TEST(Programmer_get_jobs_duration) {
  const std::chrono::hours duration_to_set = 1h;
  Programmer p(duration_to_set);
  ASSERT_TRUE(duration_to_set == p.get_jobs_duration());
}

TEST(IsInBounds_out_of_left_bound) {
  const uint32_t httpResonseCode = 500;
  ASSERT_FALSE(IsInBounds<uint32_t>(httpResonseCode, 501, 599));
}

TEST(IsInBounds_in_bounds) {
  const uint32_t httpResonseCode = 500;
  ASSERT_TRUE(IsInBounds<uint32_t>(httpResonseCode, 500, 599));
}

TEST(IsInBounds_out_of_right_bound) {
  const int some_value = -500;
  ASSERT_FALSE(IsInBounds<int>(some_value, -300000, -3500));
}

TEST(Several_strings_found) {
  size_t expected_result = 2;
  auto theStrings = std::vector<std::string>{"test", "one", "two", "test"};
  const auto count = ContainsTheString(
      [](const std::string &tested) { return tested == "test"; }, theStrings);
  ASSERT_EQ(count, expected_result);
}
//// 1.k ////

int main(int argc, char *argv[]) {

  //// 1.e and 1.f ////
  std::vector<std::unique_ptr<Job>> workers(2);
  workers[0] = std::make_unique<Pilot>(3h);
  workers[1] = std::make_unique<Programmer>(8h);
  //// 1.e and 1.f ////

  //// 1.g ////
  for (const auto &worker : workers) {
    worker->get_jobs_name();
    worker->get_jobs_description();
    worker->get_jobs_duration();
    worker->DoWork();
  }
  //// 1.g ////

  //// 1.h ////
  const uint32_t httpResonseCode = 500;
  IsInBounds<uint32_t>(httpResonseCode, 500, 599);
  //// 1.h ////

  //// 1.i ////
  auto theStrings = std::vector<std::string>{"one", "two", "test"};
  const auto count = ContainsTheString(
      [](const std::string &tested) { return tested == "test"; }, theStrings);
  std::cout << "The count is: " << count << std::endl;
  //// 1.i ////

  //// 1.j ////
  // Please check these tasks that I have completed for other companies when
  // looked for a job 5 years ago -
  // https://github.com/kozlovb/EthTest/blob/master/ArithmeticParser.cpp
  // https://github.com/kozlovb/TestTaskTemplate
  //// 1.j ////

  // this part of main is taken from the micro test framework project:
  mt::Runtime::args(argc, argv);

  size_t num_failed = mt::TestsManager::RunAllTests(stdout);
  if (num_failed == 0) {
    fprintf(stdout, "%s{ summary} All tests succeeded!%s\n", mt::green(),
            mt::def());
    return 0;
  } else {
    double percentage = 100.0 * num_failed / mt::TestsManager::tests().size();
    fprintf(stderr, "%s{ summary} %lu tests failed (%.2f%%)%s\n", mt::red(),
            num_failed, percentage, mt::def());
  }
  return -1;
}
