#include "job.hpp"
#include "pilot.hpp"
#include "programmer.hpp"

#include <memory>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <sstream>
#include <cassert>

// h. Create a template function that will test if the provided values are within a range.
//    Call the function and print out the inputs and result. The following should execute
//    assuming httpResonse is a uint32_t value:
//     i. IsInBounds<uint32_t>(httpResonseCode, 500, 599)
template <typename T>
bool IsInBounds (const T httpResponseCode, const T min, const T max)
{
  return httpResponseCode >= min && httpResponseCode <= max;
}

// 	i. Create a function that will take a vector of strings and a test function,
// 	It should return how many items in the collection matched the test. Then print out the result.
//        i. auto theStrings = vector<std::string> { “one”, “two”,  “test”};
//       ii. auto count = ContainsTheString([](const std::string& tested) { return tested == “test” }, theStrings);
unsigned ContainsTheString (std::function<bool (std::string const &)> test, std::vector<std::string> theStrings)
{
  return std::count_if (theStrings.begin (), theStrings.end (), std::move (test));
}

// 	j. Write any other code you feel would show off your understanding of C/C++
// 	and programming in general. Bonus: Variadic Template /w usage.
template <typename Stream, typename... X>
void printData (Stream & outputStream, X... x_args)
{
  (outputStream << ... << x_args) << std::endl;
}

int main (int argc, char ** argv)
{
  {
	// e. Dynamically allocate each type of job.
	std::shared_ptr<job> any_job_ptr = std::make_shared<job> ();
	std::shared_ptr<pilot> pilot_ptr = std::make_shared<pilot> ();
	std::shared_ptr<programmer> programmer_ptr = std::make_shared<programmer> ();

	// f. Store each instance in a collection.
	std::vector<std::shared_ptr<job>> job_collection = {
		any_job_ptr, pilot_ptr, programmer_ptr
	};

	// g. Iterate the collection and call each of the available interface functions.
	unsigned jobs_count = 0;
	for (auto const & job_item : job_collection)
	{
	  job_item->learn ();
	  job_item->work ();
	  job_item->rest ();
	  job_item->retire ();
	  ++jobs_count;
	}
	assert (jobs_count == 3);
  }

  {
	// calling function from item (h).
	uint32_t const min = 500;
	uint32_t const max = 599;
	uint32_t const httpResponseCode = 502;
	auto const is_in_bounds = IsInBounds<uint32_t> (httpResponseCode, min, max);

	std::cout << "min: " << min << std::endl;
	std::cout << "max: " << max << std::endl;
	std::cout << "httpResponseCode: " << httpResponseCode << std::endl;
	std::cout << "result: " << (is_in_bounds ? "true" : "false") << std::endl;
	assert (is_in_bounds);
  }

  {
	std::vector<std::string> theStrings = { "one", "two", "test" };
	[[maybe_unused]] auto const result = ContainsTheString ([] (const std::string& tested) {
	  return tested == "test";
	}, theStrings);
	assert (result == 1);
  }

  {
	std::stringstream stream1, stream2;
	printData (stream1, "blah", " ", 1, " ", 2, " ", 3);
	assert (stream1.str () == "blah 1 2 3\n");
	printData (stream2, true, ",", false, ",", "Hello!");
	assert (stream2.str () == "1,0,Hello!\n");
  }

  return 0;
}