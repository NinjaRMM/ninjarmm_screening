
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

// instructions do not say if this should be a pure virtual class or not, so allow it to be instantiated
class Job {
public:
  Job(const string& name, const string& description, unsigned int hoursRequired) :
    name_{name}, description_{description}, hoursRequired_{hoursRequired}
  {
  }

  string getName() const { return name_; };
  string getDescription() const { return description_; };
  int getHoursRequired() const { return hoursRequired_; };
  void DoWork() const
  {
    std::cout << "My work involves " << description_ << std::endl;
  }

  virtual ~Job()
  {
  }

private:
  const string name_;
  const string description_;
  const unsigned int hoursRequired_;

  // not supported until they are needed (also shows that unnecessary objects are not created)
  Job(const Job&) = delete;
  Job& operator=(const Job&) = delete;
  Job(Job&&) = delete;
  Job& operator=(Job&&) = delete;
};

class Programmer : public Job {
public:
  Programmer(string name, unsigned int hoursRequired) :
    Job{name, "computer programming", hoursRequired}
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
  Pilot(string name, unsigned int hoursRequired) :
    Job{name, "flying aircraft", hoursRequired}
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
bool IsInBounds(T value, T minimumValue, T maximumValue, bool print = true)
{
  bool ret = (value >= minimumValue) and (value <= maximumValue);
  if (print)
  {
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
  std::cout << "for input strings : ";
  for (const auto& word : strings) {
    std::cout << word << " ";
  }
  std::cout << ": you matched " << ret;
  std::cout << ((ret == 1) ? " time." : " times.") << std::endl;
  return ret;
}

template<typename T>
void print(T arg)
{
  std::cout << arg << std::endl;
}

template<typename T, typename... MoreArgs>
void print(T arg, MoreArgs... args)
{
  std::cout << arg << ' ';
  print(args...);
}

int main()
{
  std::vector<std::unique_ptr<Job>> jobs;
  jobs.emplace_back(std::make_unique<Job>("Eric", "just standard job stuff", 40));
  jobs.emplace_back(std::make_unique<Programmer>("Zawinski", 10000));
  jobs.emplace_back(std::make_unique<Pilot>("Captain Tony", 30000));

  for (const auto& iter : jobs)
  {
    std::cout << "Job name " << iter->getName() << ":" << std::endl;
    std::cout << "Description is " << iter->getDescription() << std::endl;
    std::cout << "Hours required for job " << iter->getName() <<
      " is " << iter->getHoursRequired() << std::endl;
    iter->DoWork();
    std::cout << std::endl;
  }

  const uint32_t httpResponseCode = 404;
  const uint32_t minValue = 500;
  const uint32_t maxValue = 599;
  assert(not IsInBounds<uint32_t>(httpResponseCode, minValue, maxValue));
  assert(IsInBounds(99, 45, 99));
  assert(not IsInBounds(100, 45, 99));
  const char characterZ = 'Z'; // ascii value is 90
  assert(not IsInBounds<char>(characterZ, 45, 80));
  const char characterJ = 'J'; // ascii value is 74
  assert(IsInBounds<char>(characterJ, 45, 80));
  std::cout << std::endl;

  // more complete test
  for (uint32_t value = 0; value <= 1000; ++value) {
    if ((value >= minValue) and (value <= maxValue))
    {
      assert(IsInBounds<uint32_t>(value, minValue, maxValue, false));
    }
    else
    {
      assert(not IsInBounds<uint32_t>(value, minValue, maxValue, false));
    }
  }

  auto theStrings = vector<std::string> { "one", "two", "test"};
  auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
  assert(count == 1);

  auto matchFunction = [](const std::string& tested) { return tested == "test"; };
  auto moreStrings = std::vector<std::string> { "one", "test", "two", "test"};
  assert(ContainsTheString(matchFunction, moreStrings) == 2);
  assert(ContainsTheString(matchFunction,{ "one", "two"}) == 0);

  std::cout << std::endl << "Variadic template example:" << std::endl;
  print("the", "quick", "brown", "fox", "jumped", "over", "the", "lazy", "dog");

  return 0;
}
