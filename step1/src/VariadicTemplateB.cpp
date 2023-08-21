#include <iostream>
#include <sstream>
#include <vector>

template<typename T>
std::string toStringImpl(const T& t) {
  std::stringstream stream;
  stream << t;
  return stream.str();
}

template<typename ... P>
std::vector<std::string> toString(const P& ... p) {
  return { toStringImpl(p) ... }; // this expands to a call to toStringImpl
                                  // for each passed parameter and doesn't
                                  // overaload toString a method for each 'pealed off'
                                  // parameter combination 
}

int main() {
  const std::vector<std::string> vec = toString(42, "foo", 1.6180, true);

  for (const auto &s : vec) {
    std::cout << s << '\n';
  }
}

