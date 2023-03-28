
// ran at https://cpp.sh:
//    C++20
//    -Wall, -Wextra, -Wpedantic
//    no optimization
// also ran locally with:
// clear && clang++ -std=c++17 -Wall step1.cc && ./a.out && echo $?
//    Apple clang version 12.0.5 (clang-1205.0.22.11)
//    Target: arm64-apple-darwin20.1.0

#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::string;
using std::vector;

class Job {
public:
  Job(string name, string description, unsigned int hoursRequired) :
    name_{name}, description_{description}, hoursRequired_{hoursRequired}
  {
  }

  Job(const Job&) = default;
  Job& operator=(const Job&) = default;
  Job(Job&&) = default;
  Job& operator=(Job&&) = default;

  string getName() const { return name_; };
  string getDescription() const { return description_; };
  int getHoursRequired() const { return hoursRequired_; };
  string DoWork() const
  {
    return string("My work involves " + description_);
  }

  virtual ~Job()
  {
  }

private:
  string name_;
  string description_;
  unsigned int hoursRequired_;
};

class Programmer : public Job {
public:
  Programmer(string name, string description, unsigned int hoursRequired) :
    Job{name, description, hoursRequired}
  {
  }

  ~Programmer()
  {
  }

private:
  Programmer(const Programmer&) = delete;
  Programmer& operator=(const Programmer&) = delete;
  Programmer(const Programmer&&) = delete;
  Programmer& operator=(Programmer&&) = delete;
};

class Pilot : public Job {
public:
  Pilot(string name, string description, unsigned int hoursRequired) :
    Job{name, description, hoursRequired}
  {
  }

  ~Pilot()
  {

  }

private:
  Pilot(const Pilot&) = delete;
  Pilot& operator=(const Pilot&) = delete;
  Pilot(const Pilot&&) = delete;
  Pilot& operator=(Pilot&&) = delete;
};

// returns true if value is in the range of minimumValue and maximumValue (inclusive)
// assume we want inclusive
// assume we can use beyond numeral values here
template<typename T>
bool IsInBounds(T value, T minimumValue, T maximumValue)
{
  bool ret = (value >= minimumValue) and (value <= maximumValue);
  if (ret)
  {
    std::cout << value << " is within the range of " << minimumValue <<
      " and " << maximumValue << " (inclusive)" << std::endl;
  }
  else
  {
    std::cout << value << " is NOT within the range of " << minimumValue <<
      " and " << maximumValue << " (inclusive)" << std::endl;
  }
  return ret;
}

unsigned int ContainsTheString(bool (*testFunction)(const std::string& tested), std::vector<std::string> strings)
{
  unsigned int ret = 0;
  for (std::string& string : strings)
  {
      if (testFunction(string))
      {
        ++ret;
      }
  }
  return ret;
}

int main()
{
  std::vector<Job> jobs;
  jobs.push_back(Job("eric", "just standard job stuff", 40));
  jobs.push_back(Programmer("zawinski", "hacking", 10000));
  jobs.push_back(Pilot("captain tony", "flying a plane for american airlines", 30000));

  for (const auto& iter : jobs)
  {
    std::cout << "Job name " << iter.getName() << ":" << std::endl;
    std::cout << "Description is " << iter.getDescription() << std::endl;
    std::cout << "Hours required for job " << iter.getName() <<
      " is " << iter.getHoursRequired() << std::endl;
    std::cout << iter.DoWork() << std::endl;
    std::cout << std::endl;
  }

  uint32_t httpResponseCode = 404;
  assert(not IsInBounds<uint32_t>(httpResponseCode, 500, 599));
  assert(IsInBounds(99, 45, 99));
  assert(not IsInBounds(100, 45, 99));
  char characterTest = 'Z'; // ascii value is 90
  assert(not IsInBounds<char>(characterTest, 45, 80));
  characterTest = 'J'; // ascii value is 74
  assert(IsInBounds<char>(characterTest, 45, 80));
  std::cout << std::endl;

  auto theStrings = vector<std::string> { "one", "two", "test"};
  auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
  assert(count == 1);

  auto matchFunction = [](const std::string& tested) { return tested == "test"; };
  auto moreStrings = std::vector<std::string> { "one", "test", "two", "test"};
  assert(ContainsTheString(matchFunction, moreStrings) == 2);
  assert(ContainsTheString(matchFunction,{ "one", "two"}) == 0);

  return 0;
}
