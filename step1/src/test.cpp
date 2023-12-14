#include "test.hpp"

#include <algorithm>

uint32_t containsTheString(
  std::function<bool(const std::string&)> test,
  const std::vector<std::string>&         theStrings
) {
  // Without ranges
  // uint32_t count = 0;

  // for (const auto& string : theStrings) {
  //   if (test(string)) {
  //     count++;
  //   }
  // }

  // return count;

  return std::ranges::count_if(theStrings, test);
}
