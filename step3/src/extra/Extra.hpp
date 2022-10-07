#ifndef EXTRA_HPP
    #define EXTRA_HPP

// Standard Library Headers
#include <iostream>
#include <string>

namespace extra {

/// Find the greater number between many numbers
template <typename... Ts>
decltype(auto) MaxNumber(Ts... args)
{
    std::cout << "MaxNumber() - Total of arguments passed: "
              << sizeof...(args)
              << std::endl;

    // Parameter pack expands to an initializer list
    auto r = std::max({args...});
    return r;
}

/// Fold Expression with parameter pack expansion
template <typename... Ts>
auto LeftFoldBinaryOpSum(Ts... args)
{
    // Evaluation order is: (((value + arg1) + arg2) + arg3) + ...
    // Preferred syntax for Binary fold expressions
    return (0 + ... + args);
}

}// End of namespace extra
#endif
