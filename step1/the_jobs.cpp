/*
  g++ -std=c++17 -I . -o the_jobs.out the_jobs.cpp

  - To run the tests with info, run:

  ./the_jobs.out -s
*/

#include <string>
#include <functional>

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

class Pilot : public Job
{
 public:
  Pilot() noexcept
   : Job{"pilot", "flying in the world", 48} {}
};

TEST_CASE("do work for a Pilot")
{
  SUBCASE("pilot do work")
  {
    // compile-time tested as:
    static_assert(std::is_default_constructible<Pilot>::value == true);
    // run-time tested as:
    Pilot p;
    MESSAGE("DoWork as programer is: " << p.do_work());
    CHECK(p.get_name() == "pilot");
    CHECK(p.get_description() == "flying in the world");
    CHECK(p.get_hours_required() == 48);
    CHECK(p.do_work() == "My work involves " + p.get_description());
  }
}

template<typename Container, typename Func>
void ss_map_operation(std::stringstream& ss, Container& c, Func&& f)
{
  std::for_each(
    c.begin(),
    c.end(),
    [&ss,&f](auto job) { ss << f(job) << std::endl; }
  );
}
TEST_CASE("Jobs, containers and dynamic allocation")
{
  SUBCASE("Manual Old-school allocation")
  {
    Programmer* prg_ptr = new Programmer();
    Pilot* plt_ptr = new Pilot();

    std::vector<Job*> ptr_jobs;
    ptr_jobs.push_back(prg_ptr);
    ptr_jobs.push_back(plt_ptr);
    // test job names
    std::stringstream ss;
    using namespace std::placeholders;
    ss_map_operation(ss, ptr_jobs, std::bind(&Job::get_name, _1));
    std::string job_name1, job_name2;
    ss >> job_name1 >> job_name2;
    CHECK(job_name1 == prg_ptr->get_name());
    CHECK(job_name2 == plt_ptr->get_name());
    // test job descriptions
    ss = std::stringstream(); // reset the ss
    ss_map_operation(ss, ptr_jobs, std::bind(&Job::get_description, _1));
    std::string job_desc1, job_desc2;
    std::getline(ss, job_desc1);
    CHECK(job_desc1 == prg_ptr->get_description());
    std::getline(ss, job_desc2);
    CHECK(job_desc2 == plt_ptr->get_description());
    // test job hours required
    ss = std::stringstream(); // reset the ss
    ss_map_operation(ss, ptr_jobs, std::bind(&Job::get_hours_required, _1));
    int job_hours1, job_hours2;
    ss >> job_hours1 >> job_hours2;
    CHECK(job_hours1 == prg_ptr->get_hours_required());
    CHECK(job_hours2 == plt_ptr->get_hours_required());
    // clean memory
    delete prg_ptr; prg_ptr = nullptr;
    delete plt_ptr; plt_ptr = nullptr;
  }
}