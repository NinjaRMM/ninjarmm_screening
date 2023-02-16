/*

Joao Ferreira:   devcicerojr@gmail.com
Step5 ninjarmm_screening 

Code built and tested in two different environments: MSVC tools compiler (cl.exe) AND www.onlinegdb.com
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

// Joao Ferreira: Auxiliar function that receives an r-value and returns an l-value variable of T type.
template <typename T>
T& unmove(T&& t) {return t;}

// Joao Ferreira:  Here I had to change the template function declaration, so that we now have
// different versions for different amount of arguments (2 int params ; 3 int params ; 3 int params + total)
template <typename T, typename U>
T checkIfPositive(const U& u, const int& param1, const int& param2)
{
    int param3 = 0;
    // Consider I am keeping the same EXISTING CODE from the original template function declaration 
    /*

    SOME EXISTING CODE

    */
    
    
    return u.getResult(param1, param2, param3);
}

template <typename T, typename U>
T checkIfPositive(const U& u, const int& param1, const int& param2, const int& param3)
{
    
    // Consider I am keeping the same EXISTING CODE from the original template function declaration 
    /*

    SOME EXISTING CODE

    */

    return u.getResult(param1, param2, param3);
}

template <typename T, typename U>
T checkIfPositive(const U& u, const int& param1, const int& param2, const int& param3, int& total)
{
    total = param1 + param2 + param3;
    return checkIfPositive<T,U>(u, param1, param2, param3);
}

// Joao Ferreira: Created some Template specializations needed
template <>
bool checkIfPositive<bool,OBJ_INT>(const OBJ_INT& u, const int& param1, const int& param2)
{
    return checkIfPositive<bool,OBJ_INT>(u, param1, param2, 0, unmove<int>(0));
}

template <>
int checkIfPositive<int,OBJ_INT>(const OBJ_INT& u, const int& param1, const int& param2)
{
    return checkIfPositive<int,OBJ_INT>(u, param1, param2, 0, unmove<int>(0));
}

template <>
std::string checkIfPositive<std::string,OBJ_STR>(const OBJ_STR& u, const int& param1, const int& param2)
{
    return checkIfPositive<std::string,OBJ_STR>(u, param1, param2, 0, unmove<int>(0));
}
// Joao Ferreira: End of Template specializations



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

