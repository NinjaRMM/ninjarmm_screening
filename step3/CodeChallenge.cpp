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
    std::string operator()(const bool is_gte_zero) noexcept {
        return (is_gte_zero ? "true" : "false");
    }
};

template<>
struct ResultTransformPolicy<OBJ_INT> {
    int operator()(const bool is_gte_zero) noexcept {
        return (is_gte_zero ? 1 : 0);
    }
};

template <typename T, typename U,
typename ComparePolicy = GreaterPolicy,
typename OpPolicy = SumPolicy,
typename ResultPolicy =  ResultTransformPolicy<T> >
class performOp {
    public:

    T operator()(const int param1, const int param2, const int param3) noexcept {
        return static_cast<T>(getResult(param1, param2, param3));
    }

    T operator()(const int param1, const int param2, const int param3, int& total) noexcept {
        return static_cast<T>(getResult(param1, param2, param3, total));
    }

    private:
    using OBJ_RET = decltype(std::declval<U>().getResult(0,0,0));

     int performOperation(const int param1, const int param2, const int param3) {
        return OpPolicy()(param1, param2, param3);
    }

     OBJ_RET transformResult(const int total) noexcept {
        return ResultTransformPolicy<U>()(ComparePolicy()(total));
    }

     OBJ_RET getResult(const int param1, const int param2, const int param3, int& total) noexcept {
        total = performOperation(param1, param2, param3);
        return transformResult(total);
    }

     OBJ_RET getResult(const int param1, const int param2, const int param3) noexcept {
        const auto total = performOperation(param1, param2, param3);
        return transformResult(total);
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

    iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2);
    std::cout << iResult <<  std::endl;

    iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, -3);
    std::cout << iResult << std::endl;

    iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, 3, total); //<-- Total is output param
    std::cout << iResult << " " << total << std::endl;
    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2);
    std::cout << sResult <<  std::endl;

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, -3);
    std::cout << sResult <<  std::endl;

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, 3, total); //<-- Total is output param
    std::cout << sResult << " " << total << std::endl;

    return 0;

}

