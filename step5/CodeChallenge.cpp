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

//Here's a template function that can accept a sample object and 3 integer parameters. It checks if the sum of the integer params is positive or not.
//The third integer param is optional so it's declared as a default param
//Inside the function is some existing code that needs to exist, but not relevant to this exercise. 

//--- using <optional> introduced in C++ 17 (if I recall correctly) it works perfectly with https://cpp.sh/ with C++20
#include <optional>
#include <functional>

template <typename T, typename U>
T checkIfPositive(const U& u, const int& param1, const int& param2, int param3 = 0, std::optional<std::reference_wrapper<int>> total = {})
{

    //Assume there's already some existing code in here, some business logic that we are not interested in this exercise. 
    /*

    SOME EXISTING CODE

    */
    
    if (total)
        total->get() = param1 + param2 + param3;
    return u.getResult(param1, param2, param3);
}
//--- using <optional> end

//--- In case C++ 17 or higher are not available this can be used (using template/function overload it works perfectly with https://cpp.sh/

//template <typename T, typename U>
//T checkIfPositive(const U& u, const int& param1, const int& param2, int param3, int& total) // this will be called with 4 params
//{

    //// Assume there's already some existing code in here, some business logic that we are not interested in this exercise. 
    ///*

    //SOME EXISTING CODE

    //*/
    
    //total = param1 + param2 + param3;
    //return u.getResult(param1, param2, param3);
//}

//template <typename T, typename U>
//T checkIfPositive(const U& u, const int& param1, const int& param2, int param3 = 0) // this will be called with 2 o 3 params
//{
    //int dummyTotal; // used to pass as int& param
    
    ////call function with 4 params and that has the existing code (commented out)
    //return checkIfPositive<T, U>(u, param1, param2, param3, dummyTotal); 
//}
//--- no c++ 17 or higher end

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

