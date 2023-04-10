/*
  g++ -std=c++17 -I . -o the_jobs.out the_jobs.cpp
*/

#include <string>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

class Job
{
 private:
  std::string _name{};
  std::string _description{};
  int _hours_req{0};

 public:

  std::string get_name() const;
  std::string get_description() const;
  int get_hours_required() const;

  void DoWork() const;
};

std::string Job::get_name() const { return _name; }
std::string Job::get_description() const { return _description; }
int Job::get_hours_required() const { return _hours_req; }

TEST_CASE("create job")
{
  SUBCASE("default constructor")
  {
    // compile-time tested as:
    static_assert(std::is_default_constructible<Job>::value == true);
    static_assert(std::is_nothrow_default_constructible<Job>::value == true);
    // run-time tested as:
    Job j;
    REQUIRE(j.get_name() == "");
    REQUIRE(j.get_description() == "");
    REQUIRE(j.get_hours_required() == 0);
  }
}