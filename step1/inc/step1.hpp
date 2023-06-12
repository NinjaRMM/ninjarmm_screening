/**
 * @file step1.hpp
 * @author Guilherme El Kadri Ribeiro (guilhermeelkadri@gmail.com)
 * @brief Test material NinjaRMM - Step 1
 * @version 0.1
 * @date 2023-06-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <iostream>
#include <vector>

/**
 * @brief Job class
 * 
 */
class Job 
{
public:
    virtual void action1() = 0;
    virtual void action2() = 0;
    virtual void action3() = 0;
    virtual void action4() = 0;
};

/**
 * @brief Programmer class
 * 
 */
class Programmer : public Job 
{
public:
    void action1() override;
    void action2() override;
    void action3() override;
    void action4() override;
};

/**
 * @brief Pilot class
 * 
 */
class Pilot : public Job 
{
public:
    void action1() override;
    void action2() override;
    void action3() override;
    void action4() override;
};

/**
 * @brief Checks if a value is within the specified bounds.
 * 
 * @tparam T The type of the value and bounds. 
 * @param value The value to check. 
 * @param lowerBound The lower bound of the range. 
 * @param upperBound The upper bound of the range. 
 * @return true if the value is within the bounds (inclusive), false otherwise
 */
template <typename T>
inline bool IsInBounds(T value, T lowerBound, T upperBound) 
{
    return value >= lowerBound && value <= upperBound;
}

/**
 * @brief Counts the number of strings in a collection that satisfy a given condition.
 * 
 * @tparam Func The type of the function used to test each string.
 * @param testFunction The function used to test each string. 
 * It should accept a string as a parameter and return a boolean value.
 * @param collection The collection of strings to be tested.
 * @return The count of strings in the collection that satisfy the given condition
 */
template <typename Func>
inline int ContainsTheString(Func testFunction, const std::vector<std::string>& collection) 
{
    int count = 0;
    for (const std::string& item : collection) 
    {
        if (testFunction(item)) {
            count++;
        }
    }
    return count;
}
