/*

NINJARMM Code Challenge
 
Please review the below code. 
We do expect you to execute this code. 

This file contains the 'main' function. Program execution begins and ends there.
*/

#include <iostream>
#include <cassert>

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

//Here's a template function that can accept a sample object and 3 integer parameters. It checks if the sum of the integer params is positive or not.
//The third integer param is optional so it's declared as a default param
//Inside the function is some existing code that needs to exist, but not relevant to this exercise. 
template <typename T, typename U>
T checkIfPositive(const U& u, const int& param1, const int& param2, int param3 = 0)
{

    // Assume there's already some existing code in here, some business logic that we are not interested in this exercise. 
    /*

    SOME EXISTING CODE

    */

    return u.getResult(param1, param2, param3);
}

template <typename T, typename U>
T checkIfPositive(const U& u, const int& param1, const int& param2, const int& param3, int& sum)
{
    sum = param1 + param2 + param3;
    return checkIfPositive<T,U>(u, param1, param2, param3);
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
    //Ensuring the old behaviour is not being changed...
    assert((checkIfPositive<bool, OBJ_INT>(objInt, 1, 2) == 1));
    assert((checkIfPositive<bool, OBJ_INT>(objInt, 1, -2) == 0));
    assert((checkIfPositive<bool, OBJ_INT>(objInt, 1, -2, 3) == 1));
    assert((checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2) == "true"));
    assert((checkIfPositive<std::string, OBJ_STR>(objStr, 1, -2) == "false"));
    assert((checkIfPositive<std::string, OBJ_STR>(objStr, 1, -2, 3) == "true"));


    //Now here's a new requirement where you need to get the total as an output param of checkIfPositive() template function
    //You can't touch the previous calls to checkIfPositive because they are used in a lot of different places throughout the code base.
    //How are you going to solve this problem? 
    //You can only change the code inside the EDITABLE SECTION OF CODE
    //Hint, the following calls should now be valid moving forward:
    int total = 0;

    iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2);
    std::cout << iResult <<  std::endl;
    assert(iResult == 1);

    iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, -3);
    std::cout << iResult << std::endl;
    assert(iResult == 0);

    iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, 3, total); //<-- Total is output param
    std::cout << iResult << " " << total << std::endl;
    assert((iResult == 1 && total == 6));

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2);
    std::cout << sResult <<  std::endl;
    assert(sResult == "true");

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, -3);
    std::cout << sResult <<  std::endl;
    assert(sResult == "false");

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, 3, total); //<-- Total is output param
    std::cout << sResult << " " << total << std::endl;
    assert((sResult == "true" && total == 6));

    return 0;

}

