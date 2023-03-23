/*

NINJARMM Code Challenge

Please review the below code.
We do expect you to execute this code.

This file contains the 'main' function. Program execution begins and ends there.
*/

#include <iostream>

typedef struct SampleObject1
{
    int getResult(const int& param1, const int& param2, const int& param3) const
    {
        return (param1 + param2 + param3 > 0) ? 1 : 0;
    }
}OBJ_INT;

typedef struct SampleObject2
{
    std::string getResult(const int& param1, const int& param2, const int& param3) const
    {
        return (param1 + param2 + param3 > 0) ? "true" : "false";
    }

}OBJ_STR;

OBJ_INT objInt;
OBJ_STR objStr;


//<<<<<<<< START OF EDITABLE SECTION OF CODE <<<<<<<<<<<

struct SumPolicy {
    int operator()(const int param1, const int param2, const int param3) noexcept {
        return (param1 + param2 + param3);
    }
};

// assuming I don't have access to std::greater
struct GreaterPolicy {
    bool operator()(const int result) noexcept {
        return result > 0;
    }
};

template <typename U>
struct ResultTransformPolicy {
    U operator()(const bool is_gte_zero) noexcept;
};

template<>
struct ResultTransformPolicy<OBJ_STR> {
    using OBJ_RET = decltype(std::declval<OBJ_STR>().getResult(0,0,0));
    static_assert(std::is_same_v<OBJ_RET, std::string>, "getResult() must return a std::string");

    std::string operator()(const bool is_gte_zero) noexcept {
        return (is_gte_zero ? "true" : "false");
    }
};

template<>
struct ResultTransformPolicy<OBJ_INT> {
    using OBJ_RET = decltype(std::declval<OBJ_INT>().getResult(0,0,0));
    static_assert(std::is_same_v<OBJ_RET, int>, "getResult() must return an int");

    int operator()(const bool is_gte_zero) noexcept {
        return (is_gte_zero ? 1 : 0);
    }
};

template <
    typename T,
    typename U,
    typename ComparePolicy = GreaterPolicy,
    typename OpPolicy = SumPolicy,
    typename ResultPolicy =  ResultTransformPolicy<U>
>
class performOp {
    public:

    T operator()(const int param1, const int param2, const int param3) noexcept {
        const auto total = OpPolicy()(param1, param2, param3);
        const auto cmp_result = ComparePolicy()(total);
        const auto retVal = ResultPolicy()(cmp_result);

        return static_cast<T>(retVal);
    }

    T operator()(const int param1, const int param2, const int param3, int& total) noexcept {
        total = OpPolicy()(param1, param2, param3);
        const auto cmp_result = ComparePolicy()(total);
        const auto retVal = ResultTransformPolicy<U>()(cmp_result);

        return static_cast<T>(retVal);
    }
};

//Here's a template function that can accept a sample object and 3 integer parameters. It checks if the sum of the integer params is positive or not.
//The third integer param is optional so it's declared as a default param
//Inside the function is some existing code that needs to exist, but not relevant to this exercise.
template <typename T, typename U>
[[nodiscard]] T checkIfPositive(const U& u, const int& param1, const int& param2, int param3 = 0)
{
   return performOp<T,U>()(param1, param2, param3);
}

template <typename T, typename U>
[[nodiscard]] T checkIfPositive(const U& u, const int& param1, const int& param2, int param3, int& total)
{
    total = 0;
    return performOp<T,U>()(param1, param2, param3, total);
}

//<<<<<<<< END OF EDITABLE SECTION OF CODE <<<<<<<<<<<

int main()
{
    //<<<<<<<< START OF NON-EDITABLE SECTION OF CODE <<<<<<<<<<<

    //The following calls are already coded in a lot of places throughout the project so you can't change these
    int iResult = checkIfPositive<bool, OBJ_INT>(objInt, 1, 2);
    std::cout << iResult << std::endl;

    iResult = checkIfPositive<bool, OBJ_INT>(objInt, 1, -2);
    std::cout << iResult << std::endl;

    iResult = checkIfPositive<bool, OBJ_INT>(objInt, 1, -2, 3);
    std::cout << iResult << std::endl;

    std::string sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2);
    std::cout << sResult << std::endl;

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, -2);
    std::cout << sResult << std::endl;

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, -2, 3);
    std::cout << sResult << std::endl;

    //<<<<<<<< END OF NON-EDITABLE SECTION OF CODE <<<<<<<<<<<

    //Now here's a new requirement where you need to get the total as an output param of checkIfPositive() template function
    //You can't touch the previous calls to checkIfPositive because they are used in a lot of different places throughout the code base.
    //How are you going to solve this problem?
    //You can only change the code inside the EDITABLE SECTION OF CODE
    //Hint, the following calls should now be valid moving forward:

    int total = 0;

    // Duplicated bool return value results to conform to non-editable section
    iResult = checkIfPositive<bool, OBJ_INT>(objInt, 1, 2);
    std::cout << iResult << std::endl;
    assert(iResult == 1);

    iResult = checkIfPositive<bool, OBJ_INT>(objInt, 1, -2);
    std::cout << iResult << std::endl;
    assert(iResult == 0);

    iResult = checkIfPositive<bool, OBJ_INT>(objInt, 1, -2, 3);
    std::cout << iResult << std::endl;
    assert(iResult == 1);
    // end duplicated bool return value results

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2);
    std::cout << sResult <<  std::endl;
    assert(sResult == "true");

    iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2);
    std::cout << iResult <<  std::endl;
    assert(iResult == 1);

    iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, -3);
    std::cout << iResult << std::endl;
    assert(iResult == 0);

    iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, 3, total); //<-- Total is output param
    std::cout << iResult << " " << total << std::endl;
    assert(iResult == 1);
    assert(total == 6);

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, -3);
    std::cout << sResult <<  std::endl;
    assert(sResult == "false");

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, 3, total); //<-- Total is output param
    std::cout << sResult << " " << total << std::endl;
    assert(sResult == "true");
    assert(total == 6);

    return 0;

}

