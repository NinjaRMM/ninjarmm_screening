/*
* Step1_j.cpp
* This file covers step 'j' of 'Requirements.txt'
* Examples of operator overload, parameter pack, and variadic template.
* This program overloads operator '<<' to print a tuple with a variable size and different datatypes
*/

#include <cstddef>
#include <ostream>
#include <tuple>
#include <iostream>

template<std::size_t...> struct indexes {};

template<std::size_t N, std::size_t... Is>
struct build_indices : build_indices<N - 1, N - 1, Is...> {};

template<std::size_t... Is>
struct build_indices<0, Is...> : indexes<Is...> {};

template<class Ch, class Tr, class Tuple, std::size_t... Is>
void printTuple(std::basic_ostream<Ch, Tr>& os, Tuple const& t, indexes<Is...>) {
    using swallow = int[];
    (void)swallow {
        0, (void(os << "|" << std::get<Is>(t)), 0)...
    };
}

template<class Ch, class Tr, class... Args>
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
{
    printTuple(os, t, build_indices<sizeof...(Args)>());
    return os << "|";
}

int main()
{
    auto t = std::make_tuple(1, "Name", "Description", 3.14);
    std::cout << "Tuple contents:" << std::endl;
    std::cout << t << std::endl;
}