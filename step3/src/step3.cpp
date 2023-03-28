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

// ran at https://cpp.sh:
//    C++20
//    -Wall, -Wextra, -Wpedantic
//    no optimization
// also ran locally with:
// clear && clang++ -std=c++17 -Wall step1.cc && ./a.out && echo $?
//    Apple clang version 12.0.5 (clang-1205.0.22.11)
//    Target: arm64-apple-darwin20.1.0

void callSomeExistingCode()
{
  /*
    SOME EXISTING CODE
  */
}

//Here's a template function that can accept a sample object and 3 integer parameters. It checks if the sum of the integer params is positive or not.
//The third integer param is optional so it's declared as a default param
//Inside the function is some existing code that needs to exist, but not relevant to this exercise.
template <typename T, typename U>
T checkIfPositive(const U& u, const int& param1, const int& param2, int param3 = 0)
{
    callSomeExistingCode();
    return u.getResult(param1, param2, param3);
}

//Here's a template function that can accept a sample object and 3 integer parameters. It checks if the sum of the integer params is positive or not.
// This is similar to the 3 parameter version but also will return a sum total
// Since a total is returned, the third parameter is not optional in this version
// NOTE: A new version of the function is added here because existing code relies on the old version
//    for example, we need to support a call paremeters like this : (obj, 1, 2) with param3 being defaulted
//    we could have used the original version with total being a pointer to the output, with a default of nullptr
//      but the requested API was for total to be passed by reference
//Inside the function is some existing code that needs to exist, but not relevant to this exercise.
template <typename T, typename U>
T checkIfPositive(const U& u, const int& param1, const int& param2, const int& param3, int& total)
{
    callSomeExistingCode();
    total = param1 + param2 + param3;
    return u.getResult(param1, param2, param3);
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
