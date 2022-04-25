#include <string>
#include <memory>
#include <vector>
#include <iostream>

class Job {
private:
   std::string name;
   std::string description;
   int hours;
public:
   Job(std::string name_, std::string description_, int hours_): name(name_), description(description_), hours(hours_) {
   }

   virtual ~Job() = default;

   const std::string& getName() const {
      return name;
   }

   const std::string& getDescription() const {
      return description;
   }

   int getHours() const {
      return hours;
   }

   std::string DoWork() {
      std::string ret = "My work involves " + description;
      return ret;
   }
};

class Programmer : public Job {
public:
   Programmer(): Job("programmer", "writing a program", 3) {}
   ~Programmer() = default;
};

class Pilot : public Job {
public:
   Pilot(): Job("pilot", "flying a plane", 2) {};
   ~Pilot() = default;
};

template <typename T, typename R>
bool IsInBounds(const T& value, const R& lower, const R& upper) {
   return value > lower && value < upper;
}

template <typename F>
int ContainsTheString(F&& func, const std::vector<std::string>& lines) {
   int count = 0;
   for (auto s : lines) {
      if (func(s)) {
         count++;
      }
   }
   return count;
}

class VariadicTemplate {
public:
   static std::vector<int> ints;
   static std::vector<std::string> words;

   static void storeLog(int n) {
      ints.push_back(n);
   }
   static void storeLog(const std::string& s) {
      words.push_back(s);
   }

   static void doIt() {}

   template <typename T, typename... Types>
   static void doIt(const T& arg1, Types... args) {
      storeLog(arg1); 
      doIt(args...);
   }
};

std::vector<int> VariadicTemplate::ints = {};
std::vector<std::string> VariadicTemplate::words = {};

int main() {
   auto programmer = std::make_unique<Programmer>();
   auto pilot  = std::make_unique<Pilot>();

   std::vector<Job> jobs;
   jobs.push_back(*programmer);
   jobs.push_back(*pilot);

   for (auto& job : jobs) {
      std::cout << "[ " << job.getName() << ", " << job.getDescription() << ", "
         << job.getHours() << ", " << job.DoWork() << " ]\n";
   }

   uint32_t httpResponseCode = 5;
   bool isInBounds = IsInBounds(httpResponseCode, 500, 599);
   std::cout << "Is " << httpResponseCode << " in range? " << (isInBounds ? "true" : "false") << std::endl;

   auto theStrings = std::vector<std::string> {"one", "two", "test"};
   auto count = ContainsTheString([](const std::string& tested) {return tested == "test";}, theStrings);
   std::cout << "\"test\" matched " << count << " time(s)\n";

   std::cout << "\nVariadicTemplate" << "\n";
   VariadicTemplate::doIt(1, "one", 2, "three", 3, "two");
   std::cout << "Only numbers: ";
   for (auto n : VariadicTemplate::ints) {
      std::cout << n << " ";
   }
   std::cout << "\nOnly words: ";
   for (auto word : VariadicTemplate::words) {
      std::cout << word << " ";
   }
   std::cout << "\n";
}
