#include <iostream>
#include <vector>
#include <functional>

class Job
{
public: 

  virtual std::string getName() = 0;

  virtual std::string getDescription() = 0;

  virtual unsigned int getHours() = 0;

  virtual void DoWork()
  {
    std::cout << "My work involves " << getDescription() << "\n";
  }

};

class Programmer : public Job
{
  std::string getName() {
    return "Programmer";
  }

  std::string getDescription()
  {
    return "Drinking coffee and writing code";
  }

  unsigned int getHours() {
    return 80;
  }
};

class Pilot : public Job
{
  std::string getName() {
    return "Pilot";
  }

  std::string getDescription()
  {
    return "Keeping the plane in the sky";
  }

  unsigned int getHours() {
    return 40;
  }
};

template<typename T>
bool IsInBounds(T val, T lower, T upper)
{
  return val > lower && val < upper;
}

using Predicate = std::function<bool(const std::string&)>;
unsigned int ContainsTheString(Predicate p, std::vector<std::string> const& toBeTested)
{
  unsigned int positiveResults = 0;
  for (auto s : toBeTested)
    positiveResults += p(s);

  return positiveResults;
}

template <typename T> void print(const T& t)
{
  std::cout << t << std::endl;
}

template <typename First, typename... Remaining>
void print(const First& first, const Remaining&... remaining)
{
  std::cout << first << "\n";
  print(remaining...);
};


int main()
{
    /* Polymorphism portion */
    std::vector<Job*> jobs = { new Programmer(), new Pilot() };
    for (auto job : jobs)
    {
      std::cout << job->getName() << "\n";
      std::cout << job->getHours() << "\n";
      job->DoWork() ;
    }

    /* Template portion */
    uint32_t httpResonseCode = 520;
    std::cout << (IsInBounds<uint32_t>(httpResonseCode, 500, 599) ? "Response in bounds" : "Response not in bounds") << "\n";

    /* Predicate portion */
    auto theStrings = std::vector<std::string>{ "one", "two",  "test" };
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    std::cout << count;

    /* Variadic Template portion */
    print("printing 1 thing");
    print("printing 2 things: ", "here's the other thing");
    print("printing 3 things: ", "hers's another things", "here's the last thing");
}