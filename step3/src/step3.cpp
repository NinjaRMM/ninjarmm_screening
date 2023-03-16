//
// I run this program using VS2019 C++14 Standard
//


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
//
//Here's a template function that can accept a sample object and 3 integer parameters. It checks if the sum of the integer params is positive or not.
//The third integer param is optional so it's declared as a default param
//Inside the function is some existing code that needs to exist, but not relevant to this exercise.
// 

//
// Stop condition when the user did not provide total param aka backward compatible
// arg1 sumTotal is the sum of the 3 args for which checkIfPositive was invoked
//
void setTotal(const int sumTotal)
{
    //std::cout << "no total provided as arg4, do nothing!!\n";
}

//
// Stop condition when checkIfPositive was invoked with arg4 as outTotal
// arg1 sumTotal is the sum of the 3 args for which checkIfPositive was invoked
// arg2 outTotal is the refrence to the computed sum
//
void setTotal(const int sumTotal, int& outTotal)
{
    outTotal = sumTotal;
}

//
// Stop condition when checkIfPositive was invoked with too many args( > 4)
// arg1 sumTotal is the sum of the 3 args for which checkIfPositive was invoked
// out is the list of left overs args at index >= 4
//
template <typename... Types>
void setTotal(int sumTotal, Types&... out)
{
    std::cout << "error too many input args, try again!!!\n";
    // TBD- throw an exception
}

template <typename T, typename U , typename... Types>
T checkIfPositive(const U& u, const int& param1, const int& param2, int param3 = 0, Types&... outTotal)
{

    // Assume there's already some existing code in here, some business logic that we are not interested in this exercise.
    /*

    SOME EXISTING CODE

    */

    //
    // unpack vardiac via recusrsion, outTotal will be set with the sums of the param1-param3 if provided
    //
    setTotal(param1 + param2 + param3, outTotal...);

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
    std::cout << sResult << " " << std::endl;

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, -3);
    std::cout << sResult <<  std::endl;

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, 3, total); //<-- Total is output param
    std::cout << sResult << " " << total << std::endl;
    

    return 0;
}

