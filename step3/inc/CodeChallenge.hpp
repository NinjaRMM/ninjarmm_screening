/**
 * @file CodeChallenge.hpp
 * @author Guilherme El Kadri Ribeiro (guilhermeelkadri@gmail.com)
 * @brief Test material NinjaRMM - Step 3
 * @version 0.1
 * @date 2023-06-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <iostream>

/**
 * @brief A sample object that performs a calculation and returns the result as an integer.
 */
typedef struct SampleObject1
{
    /**
     * @brief Calculates the result based on the provided parameters.
     *
     * @param param1 The first parameter.
     * @param param2 The second parameter.
     * @param param3 The third parameter.
     * @return int The calculated result (1 if the sum of parameters is greater than 0, 0 otherwise).
     */
    int getResult(const int& param1, const int& param2, const int& param3) const
    {
        return (param1 + param2 + param3 > 0) ? 1 : 0;
    }
} OBJ_INT;

/**
 * @brief A sample object that performs a calculation and returns the result as a string.
 */
typedef struct SampleObject2
{
    /**
     * @brief Calculates the result based on the provided parameters.
     *
     * @param param1 The first parameter.
     * @param param2 The second parameter.
     * @param param3 The third parameter.
     * @return std::string The calculated result ("true" if the sum of parameters is greater than 0, "false" otherwise).
     */
    std::string getResult(const int& param1, const int& param2, const int& param3) const
    {
        return (param1 + param2 + param3 > 0) ? "true" : "false";
    }

} OBJ_STR;

/**
 * @brief Checks if the result of a calculation is positive based on the provided object and parameters.
 *
 * @tparam T The type of the result.
 * @tparam U The type of the object performing the calculation.
 * @param u The object performing the calculation.
 * @param param1 The first parameter.
 * @param param2 The second parameter.
 * @param param3 The third parameter.
 * @param total A pointer to an integer to store the sum of parameters (optional).
 * @return T The result of the calculation.
 */
template <typename T, typename U>
inline T checkIfPositive(const U& u, const int& param1, const int& param2, int param3 = 0, int* total = nullptr)
{
    T result = u.getResult(param1, param2, param3);

    if (total != nullptr) 
    {
        *total = param1 + param2 + param3;
    }
    return result;
}
