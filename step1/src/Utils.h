/*
 * Copyright (c) 2023, Eberty Alves
 */

#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>
#include <stdexcept>
#include <string>

/**
 * @brief Calculates the sum of a variable number of arguments using fold expression syntax.
 * 
 * @tparam T The type of arguments.
 *
 * @param args The arguments to sum.
 * 
 * @return The sum of the provided arguments.
*/
template <typename... T>
auto Sum(const T &... args)
{
    return (args + ...);
}

/**
 * @brief Template function that takes a 'variable number' of objects and returns the sum of their hours required.
 *
 * @tparam T Type of the objects.
 *
 * @param jobs The jobs to calculate the sum of working hours.
 *
 * @return The sum of hours required for the provided jobs.
 */
template <typename... T>
unsigned int SumHoursRequired(const T &... jobs)
{
    return Sum(jobs->GetHoursRequired()...);
}

/**
 * @brief Template function that takes a 'vector' of objects and returns the sum of their hours required.
 *
 * Equivalent to:
 *      unsigned int sum = 0;
 *      for (const auto &job : jobs)
 *          sum += job->GetHoursRequired();
 *      return sum;
 *
 * @tparam T Type of the objects in the vector.
 *
 * @param jobs Vector of objects to sum the hours required.
 *
 * @return The sum of the hours required of all Job objects in the vector.
 */
template <typename T>
unsigned int SumHoursRequired(const T &jobs)
{
    return std::accumulate(jobs.begin(), jobs.end(), 0,
                           [](auto sum, const auto &job) { return sum + job->GetHoursRequired(); });
}

/**
 * @brief Template function that checks if a value is within a range.
 * 
 * @tparam T The type of the value and bounds.
 *
 * @param value The value to check.
 * @param lowerBound Lower bound of the range (inclusive).
 * @param upperBound Upper bound of the range (inclusive).
 *
 * @throws std::invalid_argument if the lower bound is greater than the upper bound.
 *
 * @return True if the value is within the range, false otherwise.
 */
template <typename T>
bool IsInBounds(const T &value, const T &lowerBound, const T &upperBound)
{
    if (lowerBound > upperBound)
    {
        throw std::invalid_argument("Lower bound cannot be greater than upper bound.");
    }
    return (value >= lowerBound && value <= upperBound);
}

/**
 * @brief Counts the number of strings in a vector that satisfy a given test function.
 *
 * Equivalent to:
 *      int count = 0;
 *      for (const auto &str : stringList)
 *          if (testFunction(str))
 *              count++;
 *      return count;
 *
 * @param testFunction The function to apply to each string.
 * @param stringList A vector of strings to be tested.
 *
 * @throws std::invalid_argument if testFunction is a null pointer.
 *
 * @return The number of strings in the vector that satisfy the condition.
 */
int ContainsTheString(std::function<bool(const std::string &)> testFunction, const std::vector<std::string> &stringList)
{
    if (!testFunction)
    {
        throw std::invalid_argument("testFunction cannot be null.");
    }
    return static_cast<int>(std::count_if(stringList.begin(), stringList.end(), testFunction));
}

#endif // UTILS_H
