#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#ifndef UTILS_HPP
#define UTILS_HPP

enum class RangePolicy
{
    OPEN,
    LEFT_OPEN,
    RIGHT_OPEN,
    CLOSED
};

template<typename T>
std::pair<std::function<bool(T, T)>, std::function<bool(T, T)>> GetRangeOperators(RangePolicy range_policy)
{
    std::function<bool(T, T)> left_operator;
    std::function<bool(T, T)> right_operator;

    switch (range_policy)
    {
        case RangePolicy::OPEN:
            left_operator = std::greater<T>();
            right_operator = std::less<T>();
            break;

        case RangePolicy::LEFT_OPEN:
            left_operator = std::greater<T>();
            right_operator = std::less_equal<T>();
            break;

        case RangePolicy::RIGHT_OPEN:
            left_operator = std::greater_equal<T>();
            right_operator = std::less<T>();
            break;

        case RangePolicy::CLOSED:
        default:
            left_operator = std::greater_equal<T>();
            right_operator = std::less_equal<T>();
            break;
    }

    return std::make_pair(left_operator, right_operator);
}

template<typename T>
bool IsInBounds(T target, T lower_boundary, T upper_boundary)
{
    return IsInBounds(target, lower_boundary, upper_boundary, RangePolicy::CLOSED);
}

template<typename T>
bool IsInBounds(T target, T lower_boundary, T upper_boundary, RangePolicy range_policy)
{
    auto [left_operator, right_operator] = GetRangeOperators<T>(range_policy);

    return left_operator(target, lower_boundary) && right_operator(target, upper_boundary);
}

uint32_t ContainsTheString(const std::function<bool(std::string)> &test_function, const std::vector<std::string> &strings);

template<typename T, typename... Args>
int CountIf(T test_function, Args&&... args)
{
    int counter = 0;

    (void) std::initializer_list<int>
    {
        ((test_function(std::forward<Args>(args)) ? counter++ : 0), 0)...
    };

    return counter;
}

#endif // UTILS_HPP
