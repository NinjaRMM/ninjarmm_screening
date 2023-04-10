/*
  g++ -std=c++17 -I . -o the_jobs.out the_jobs.cpp

  - To run the tests with info, run:

  ./the_jobs.out -s
*/

#include <string>
#include <functional>
#include <memory>

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
  SUBCASE("Using shared_ptr allocation")
  {
    std::shared_ptr<Programmer> prg_ptr = std::make_shared<Programmer>();
    std::shared_ptr<Pilot> plt_ptr = std::make_shared<Pilot>();

    std::vector<std::shared_ptr<Job>> jobs;
    jobs.push_back(prg_ptr);
    jobs.push_back(plt_ptr);
    // test job names
    std::stringstream ss;
    using namespace std::placeholders;
    ss_map_operation(ss, jobs, std::bind(&Job::get_name, _1));
    std::string job_name1, job_name2;
    ss >> job_name1 >> job_name2;
    CHECK(job_name1 == prg_ptr->get_name());
    CHECK(job_name2 == plt_ptr->get_name());
    // test job descriptions
    ss = std::stringstream(); // reset the ss
    ss_map_operation(ss, jobs, std::bind(&Job::get_description, _1));
    std::string job_desc1, job_desc2;
    std::getline(ss, job_desc1);
    CHECK(job_desc1 == prg_ptr->get_description());
    std::getline(ss, job_desc2);
    CHECK(job_desc2 == plt_ptr->get_description());
    // test job hours required
    ss = std::stringstream(); // reset the ss
    ss_map_operation(ss, jobs, std::bind(&Job::get_hours_required, _1));
    int job_hours1, job_hours2;
    ss >> job_hours1 >> job_hours2;
    CHECK(job_hours1 == prg_ptr->get_hours_required());
    CHECK(job_hours2 == plt_ptr->get_hours_required());
  }
}

bool IsInBound(uint32_t value, uint32_t lower_bound, uint32_t upper_bound)
{
  return value < lower_bound ?
    false : upper_bound < value ?
    false : true;
}
TEST_CASE("IsInBound function to test if a value is inside a range of order comparable types")
{
  SUBCASE("Test True for inside value in range for uint32_t")
  {
    uint32_t value{100}, lower_bound{50}, upper_bound{150};
    bool is_in_bound = IsInBound(value, lower_bound, upper_bound);
    CHECK(is_in_bound == true);
  }
  SUBCASE("Test True for non-inside value in range for uint32_t")
  {
    uint32_t value{10}, lower_bound{50}, upper_bound{150};
    bool is_in_bound = IsInBound(value, lower_bound, upper_bound);
    CHECK(is_in_bound == false);
  }
  SUBCASE("Test True for value equal to lower_bound for uint32_t")
  {
    uint32_t value{50}, lower_bound{50}, upper_bound{150};
    bool is_in_bound = IsInBound(value, lower_bound, upper_bound);
    CHECK(is_in_bound == true);
  }
  SUBCASE("Test True for value equal to upper_bound for uint32_t")
  {
    uint32_t value{150}, lower_bound{50}, upper_bound{150};
    bool is_in_bound = IsInBound(value, lower_bound, upper_bound);
    CHECK(is_in_bound == true);
  }
}