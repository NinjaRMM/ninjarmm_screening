#ifndef ASKED_HPP
    #define ASKED_HPP

// Standard Library Headers
#include <algorithm>
#include <functional>
#include <vector>

// Application Headers
#include "src/oop/Job.hpp"

/// Type aliases
using Hof = std::function<bool (const std::string&)>;
using VecStr = std::vector<std::string>;

namespace util {

/// Check if a value is inside a passed range
template <typename T>
requires std::is_arithmetic_v<T>
void IsInBounds(const T& value, const T& min, const T& max)
{
    std::cout << "Min: "   << min   << std::endl;
    std::cout << "Max: "   << max   << std::endl;
    std::cout << "Value: " << value << std::endl;

    if (value < min || value > max) {
        std::cout << "Value is OUT OF RANGE" << std::endl;
    }
    else {
        std::cout << "Value is IN RANGE" << std::endl;
    }
}

/// High Order Function to test against a given function
int ContainsTheString(const Hof& func, const VecStr& vec);

/// Iterate over a collection of Jobs
void DisplayJobsInfo(const job::VecJob& jobs);

/// Display "***" on the standard output
void PrintSeparator();

}// End of namespace util
#endif
