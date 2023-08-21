#include <functional>
#include <string>
#include <vector>

size_t ContainsTheString(std::function<bool(const std::string&)> f, std::vector<std::string> vector) {
  size_t count = 0;
  for (const auto &element : vector) {
    count = count + 1 * f(element); // rellying on casting so to avoid branching
  }
  return count;
}
