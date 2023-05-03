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
template <typename T, typename U>
T checkIfPositive(const U& u, const int& param1, const int& param2, int param3 = 0)
{

    // Assume there's already some existing code in here, some business logic that we are not interested in this exercise. 
    /*

    SOME EXISTING CODE

    */

    return u.getResult(param1, param2, param3);
}

// Please see comments on line 92
// I also have noticed that although param3 has a different argument type (non-constant copy), it's being used as a const int in the other functions.
template <typename T, typename U>
T checkIfPositive(const U& u, const int& param1, const int& param2, int param3, int &total)
{
    total = param1 + param2 + param3;
    return checkIfPositive<T, U>(u, param1, param2, param3);
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

    /*
     Since the instructions explicitly state that we can only change the code inside the "EDITABLE SECTION OF CODE",
     it's best to leave the code below untouched, even is 'not' part of NON-EDITABLE SECTION OF CODE (see line 83).

     However, regarding the instructions, which state that we cannot touch the previous calls to checkIfPositive,
     it's important to note that these instructions only apply to the previous calls in the provided code snippet (code base),
     and not to future calls that we may need to add (lines 145 and 154).

     Therefore, it would be acceptable to modify the calls to checkIfPositive that were presented in the code below,
     as long as we don't modify the calls in other parts of the codebase.

     With that said, it would be a good idea to modify the following calls:

        iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, 3, total);
        sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, 3, total);
    to
        iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, 3, &total);
        sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, 3, &total);

     since using a pointer directly as output is more intuitive and common in C++.
     This would avoid misunderstandings regarding the output param and other input variables.

     We can improve the code by modifying the original checkIfPositive function in the editable section to include
     the total output, so that we don't need to create a new function:

    ```
        template <typename T, typename U>
        T checkIfPositive(const U &u, const int &param1, const int &param2, int param3 = 0, int *total = nullptr)
        {
            int sum = param1 + param2 + param3;

            if (total != nullptr)
            {
                *total = sum;
            }

            // SOME EXISTING CODE

            return u.getResult(param1, param2, param3);
        }
    ```

     This is my best answer.

     It's important to note that the modifications suggested above will not alter the behavior of the existing calls to checkIfPositive in the codebase.
     These modifications only affect future calls that require the output parameter (lines 145 and 154).
    */

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

