/*

NINJARMM Code Challenge
 
Please review the below code. 
We do expect you to execute this code. 

This file contains the 'main' function. Program execution begins and ends there.
*/

// Ninja8 Screening test
// Daniel Vasconcelos Gomes July 2022
// daniel.vasconcelosgomes@gmail.com
// https://github.com/zlogdanbr/ninjarmm_screening
// +55 11 983906930
// I used c++17 because it is easier to dabble with variadic templates
// I could not think of any other solution besides using variadic templates.
// building this with C++14 will throw some compiler warnings

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

// renamed old function still used 
template <typename T, typename U>
T checkIfPositive_impl(const U& u, const int& param1, const int& param2, int param3 = 0)
{
    return u.getResult(param1, param2, param3);
}

// new function now accepts extra parameters using variadic templates
template <typename T, typename U, typename...Args>
T checkIfPositive( const U& u, const int& param1, const int& param2, int param3 = 0, Args&...params)
{
	
    // Assume there's already some existing code in here, some business logic that we are not interested in this exercise. 
    /*

    SOME EXISTING CODE

    */
	
	
	static const unsigned short int size = sizeof...(params);
	
	// I used C++17 folding
	// https://stackoverflow.com/questions/70079694/how-to-access-variadic-variable-in-variadic-template-function
	if ( size != 0 )
	{
		((params=param1+param2+param3),...);
	}

    return checkIfPositive_impl<T,U>(u,param1,param2,param3);
	

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
