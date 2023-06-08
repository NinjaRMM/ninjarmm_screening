#pragma once

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

namespace ninjarmm_screening_step1
{

/* General utilities */

template<typename T>
bool IsInBounds(const T& value, const T& min, const T& max)
{   
    return (value >= min && value <= max);
}


using matchStringFunction = bool(*)(const string& str);

int ContainsTheString(matchStringFunction function, const vector<string>& theStrings)
{   
    return count_if (theStrings.begin(), theStrings.end(), function);
}

/* Using variadic template and C++17 to create a fold expression */
template<typename... Args>
auto SumVariadicArguments(Args... args)
{
    return(... + args);
}

}