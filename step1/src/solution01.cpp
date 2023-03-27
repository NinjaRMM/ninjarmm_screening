//////////////////////////////////////////////////////////////////////
// This code is an implementation for the ninjarmm screening.
//
// How to compile:
//   g++ -std=c++11 solution01.cpp -o solution01
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <stdint.h>
#include <stdarg.h>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Job interface definition
//////////////////////////////////////////////////////////////////////
class Job {
public:
  virtual string GetName() = 0; 
  virtual string GetDescription() = 0;
  virtual unsigned int GetHoursRequired() = 0;
  virtual void DoWork() = 0;
  virtual ~Job() = default;
};

//////////////////////////////////////////////////////////////////////
// Programmer Class Implementation
//////////////////////////////////////////////////////////////////////
class Programmer : public Job {
public:
  string GetName() override {
    return "Programmer";
  }

  string GetDescription() override {
    return "writing computer programs.";
  }

  unsigned int GetHoursRequired() override {
    return 8;
  }

  void DoWork() override {
    cout << "My work involves " + GetDescription() << endl;
  }
};

//////////////////////////////////////////////////////////////////////
// Pilot Class Implementation
//////////////////////////////////////////////////////////////////////
class Pilot : public Job { 
public:
  string GetName() override {
    return "Pilot";
  }

  string GetDescription() override {
    return "flying an airplane.";
  }

  unsigned int GetHoursRequired() override {
    return 11;
  }

  void DoWork() override {
    cout << "My work involves " + GetDescription() << endl;
  }
};

//////////////////////////////////////////////////////////////////////
// Template function to test if a provided value are within a range 
//////////////////////////////////////////////////////////////////////
template <typename T>
bool IsInBounds(T value_to_test, T min_range, T max_range) {
  bool result = false;
  cout << "Function IsInBounds activated ..." << endl;
  cout << "  - Parameter supplied: " << value_to_test << endl;
  cout << "  - Min value accepted: " << min_range << endl;
  cout << "  - Max value accepted: " << max_range << endl;

  if( value_to_test >= min_range && value_to_test <= max_range ) {
    result = true;
  }

  cout << "  - Test Result.......: ";
  if( result == true) {
    cout << "[Accepted]";
  } else {
    cout << "[ERROR]";
  }
  cout << endl;

  return result;
};

//////////////////////////////////////////////////////////////////////
// Function to check how many items matched the collection provided on a vector
//////////////////////////////////////////////////////////////////////
int ContainsTheString( bool (*test_function)(const string&), const vector<string> &vector_parameter ) {
  int counter = 0;

  for( const auto &item : vector_parameter) {
    if( test_function(item) ) {
      counter++;
    }
  }

  return counter;
}

//////////////////////////////////////////////////////////////////////
// This routine sums the arguments using variadic functions
//////////////////////////////////////////////////////////////////////
int sum(int number_of_parameters, ...) {
  int total = 0;
  va_list ptr;
  va_start(ptr, number_of_parameters);

  for(int i=0 ; i < number_of_parameters ; i++){
    total += va_arg(ptr, int);
  }

  va_end(ptr);

  return total;
}

//////////////////////////////////////////////////////////////////////
// Template function to test if a expected value is equal to a processing result
//////////////////////////////////////////////////////////////////////
template <typename T>
bool is_equal(T value_to_test, T expected_value) {
  bool result = false;

  cout << "Testing if [" << value_to_test << "] is equal to the expected value [" << expected_value << "] ... ";
  cout << "[";
  if( value_to_test == expected_value ) {
    result = true;
    cout << "SUCCESS";
  } else {
    cout << "ERROR";
  }

  cout << "]." << endl;

  return result;
}

//////////////////////////////////////////////////////////////////////
// Program Entry Point
//////////////////////////////////////////////////////////////////////
int main() {
  list<Job *> job_list;
  list<Job *>::iterator job_list_iterator;
  uint32_t value_to_check=0;

  cout << "- Creating objects ..." << endl;
  Job *programmer = new Programmer();
  Job *pilot = new Pilot();

  // Adds the Programmer object to the list
  cout << "- Adding the Programmer object to the container ..." << endl;
  job_list.push_back(programmer);

  // Adds the Pilot object to the list
  cout << "- Adding the Pilot object to the container ..." << endl;
  job_list.push_back(pilot);


  // Iterate throught the objects
  cout << endl << "- Iterating over the list and Calling objects interface functions:" << endl;
  cout << "==================================================================" << endl;
  for(Job *element : job_list) {
    cout << "Calling GetName().........: " << element->GetName() << endl;
    cout << "Calling GetDescription()..: " << element->GetDescription() << endl;
    cout << "Calling GetHoursRequired(): " << element->GetHoursRequired() << endl;
    cout << "Calling DoWork()..........: ";
    element->DoWork();
    cout << endl;
  }

  // Test if the Working hours is within a range
  cout << "Testing programmer object consistency" << endl;
  value_to_check = programmer->GetHoursRequired();
  IsInBounds<uint32_t>( value_to_check, 1, 12 );

  cout << "Testing pilot object consistency" << endl;
  value_to_check = pilot->GetHoursRequired();
  IsInBounds<uint32_t>( value_to_check, 1, 12 );
  
  // Checks how many items in the collection matched the test
  auto theStrings  = vector<std::string> { "one", "two", "test" };
  auto count = ContainsTheString([](const std::string& tested) 
  { 
    return tested == "test";
  }, theStrings);
  cout << "Number of occurrences that matched the test: " << count << endl;

  // Freestyle coding
  cout << endl << "Variadic Demonstrration:" << endl;
  auto result = sum(2, 1,2);
  cout << "Variadic sum(2, 1,2)   = " << result << endl;
  result = sum(3, 1, 2, 3);
  cout << "Variadic sum(3, 1,2,3) = " << result << endl;

  // Performs the tests for thefuncion sum
  is_equal(sum(2, 1,2), 3);
  is_equal(sum(3, 1,2,3), 6);
  is_equal(sum(1, -5), -5);
  is_equal(sum(2, -10,-20), -30);


  // Release resources
  delete programmer;
  delete pilot;

  // Returns to the operating system
  return EXIT_SUCCESS;
}

