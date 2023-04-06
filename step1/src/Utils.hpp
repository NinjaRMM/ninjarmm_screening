/**
 * @file Utils.hpp
 *
 * @brief Declaration of utils templates.
 *
 * @author   gustavo.garcia
 * @date     Apr 02, 2023
 * @revision 1
 *
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

/* INCLUDES ******************************************************************/

#include <cstdint>
#include <vector>
#include <string>

/* DEFINITIONS ***************************************************************/

template<typename T>
bool IsInBounds(const T& value, const T& lower, const T& upper) {
    std::cout << "value: " << value << ", lower: " << lower << ", upper: " << upper << std::endl;
    return value >= lower && value <= upper;
}

template<typename Predicate>
int ContainsTheString(Predicate p, const std::vector<std::string>& strings) {
    int count = 0;
    for (const auto& str : strings) {
        if (p(str)) {
            count++;
        }
    }
    return count;
}

#endif // UTILS_HPP_
