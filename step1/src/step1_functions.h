#ifndef STEP_ONE_FUNCTIONS_H
#define STEP_ONE_FUNCTIONS_H

#include <cassert>
#include <functional>
#include <string>
#include <vector>

namespace ninjaone_screening {

//Returns if value is within the bound [lower,bound, upper_bound]. (inclusive)
template<typename T>
bool IsInBounds(const T& value, const T& lower_bound, const T& upper_bound) {
	//If lower bound is higher then upper bound this always returns false.
	//maybe that is intentional maybe it is an error we'd like to notify the user of the function about
#ifdef STRICT_BOUNDS_CHECKING
	//asserts do nothin in debug mode. but a debug run may catch the problem
	assert(upper_bound >= lower_bound);
#endif 
	return (value >= lower_bound) && (value <= upper_bound);
}

#if _MSVC_LANG  >= 202002L //we have c++20
//if we wanted to limit this to arithmetic values
template<typename T> requires std::is_arithmetic_v<T>
bool IsInBoundsNumeric(const T& value, const T& lower_bound, const T& upper_bound) {
	return IsInBounds<T>(value, lower_bound, upper_bound);
}
#elif  _MSVC_LANG >= 201402L //requires c++14
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, bool> IsInBoundsNumeric(const T& value, const T& lower_bound, const T& upper_bound) {
	return IsInBounds<T>(value, lower_bound, upper_bound);
}
#endif
//Returns a count of how many strings in the_strings that satisfies functor
//i.e. functor(string) returns true
//Note: I would probably name this something more generic than ContainsTheString because
//it can test for much more than string equality. i.e. strings with length greater than 10
std::size_t ContainsTheString(
	const std::function<bool(const std::string&)>& functor
	, const std::vector<std::string> the_strings);



} //ninjaone_screening


#endif //STEP_ONE_FUNCTIONS_H
