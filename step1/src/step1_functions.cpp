#include "step1_functions.h"


#include <algorithm>


namespace ninjaone_screening {

std::size_t ContainsTheString(
	const std::function<bool(const std::string&)>& functor
	, const std::vector<std::string> the_strings) {

	std::size_t result = std::count_if(the_strings.begin(), the_strings.end(), functor);

	//Since there is a period and not a comma after test i assume this function isn't responsible for the printing
	//"It should return how many items in the collection matched the test. Then print out the result."
	return result;
}

} //namespace ninjaone_screening
