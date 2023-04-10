/*
  g++ -std=c++17 -I . -o the_jobs.out the_jobs.cpp

  - To run the tests with info, run:

  ./the_jobs.out -s
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
  Job() = default;
  Job(const std::string& name, const std::string& description, int hours)
   : _name{name}, _description{description}, _hours_req{hours} {}

  std::string get_name() const;
  std::string get_description() const;
  int get_hours_required() const;

  void DoWork() const;
  std::string do_work() const;
};

std::string Job::get_name() const { return _name; }
std::string Job::get_description() const { return _description; }
int Job::get_hours_required() const { return _hours_req; }
void Job::DoWork() const
{
  std::cout << do_work() << std::endl;
}
std::string Job::do_work() const
{
  return std::string{"My work involves "} + _description;
}

TEST_CASE("create job")
{
  SUBCASE("default constructor")
  {
    // compile-time tested as:
    static_assert(std::is_nothrow_default_constructible<Job>::value == true);
    static_assert(
      std::is_constructible<Job,std::string,std::string,int>::value == true
    );
    // run-time tested as:
    Job j;
    CHECK(j.get_name() == "");
    CHECK(j.get_description() == "");
    CHECK(j.get_hours_required() == 0);
  }
}

class Programmer : public Job
{
 public:
  Programmer() noexcept
   : Job{"programmer", "creating solutions in the world", 40} {}
};

TEST_CASE("do work for a Programmer")
{
  SUBCASE("programer do work")
  {
    // compile-time tested as:
    static_assert(std::is_default_constructible<Programmer>::value == true);
    // run-time tested as:
    Programmer p;
    MESSAGE("DoWork as programer is: " << p.do_work());
    CHECK(p.get_name() == "programmer");
    CHECK(p.get_description() == "creating solutions in the world");
    CHECK(p.get_hours_required() == 40);
    CHECK(p.do_work() == "My work involves " + p.get_description());
  }
}
