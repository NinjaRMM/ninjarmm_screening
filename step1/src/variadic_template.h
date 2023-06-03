#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#pragma once

// Base case for the variadic template recursion
// Used for the last execution of the recursion when there will be no more parameters in args
void convertToStringVector(std::vector<std::string>& result);

// Convert an object to string using std::stringstream
template<typename T>
std::string nativeObjectToString(const T& obj)
{
    std::stringstream ss;
    ss << obj;
    return ss.str();
}

// Check if the object has a "toString" method
//SFINAE technique 
template<typename T>
auto hasToStringMethod(int) -> decltype(std::declval<T>().toString(), std::true_type{});

//SFINAE technique 
template<typename T>
auto hasToStringMethod(...) -> std::false_type;

template<typename T>
constexpr bool typeHasToStringMethod = decltype(hasToStringMethod<T>(0))::value;

// Convert an object to string or add a specific message
template<typename T>
std::string convertObjectToString(const T& obj) {
    if constexpr (typeHasToStringMethod<T>) {
        return obj.toString();
    } else {
        std::string str = "toString not implemented for object from class '";
        str += typeid(obj).name();
        str += "'";
        return str;
    }
}

// Recursive function to convert objects to strings
template<typename T, typename... Args>
void convertToStringVector(std::vector<std::string>& result, const T& value, const Args&... args) {
    std::string str;

    if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, std::string>) {
        str = nativeObjectToString(value);
    } else {
        str = convertObjectToString(value);
    }

    result.push_back(str);

    convertToStringVector(result, args...);
}
