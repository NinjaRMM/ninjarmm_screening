# NINJARMM Code Challenge - Step 3

This code challenge involves modifying a template function called `checkIfPositive`. The function checks if the sum of
integer parameters passed to it is positive or not. The function takes an object of a specific type `U` and three
integer parameters as input. The third integer parameter is optional and is declared as a default parameter.

The task is to modify the `checkIfPositive` function so that it also returns the sum of the integer parameters passed to
it. To accomplish this, an additional integer parameter `total` has been added as an output parameter to
the `checkIfPositive` function. The `checkIfPositive` function should not modify the existing code within it.

## Source Code

The source code for this program is available on GitHub:

[CodeChallenge.cpp](CodeChallenge.cpp)

## Editable Section of the Code

The following is the editable section of the code where you should make the necessary changes to the `checkIfPositive`
function.

```c++
template <typename T, typename U>
T checkIfPositive(const U& u, const int& param1, const int& param2, int param3 = 0) {

    // Assume there's already some existing code in here, some business logic that we are not interested in this exercise. 
    /*

    SOME EXISTING CODE

    */

    return u.getResult(param1, param2, param3);
}
```

## Non-Editable Section of the Code

The following is the non-editable section of the code.

```c++
int main() {
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

    int total = 0;

    iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2);
    std::cout << iResult <<  std::endl;

    iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, -3);
    std::cout << iResult << std::endl;

    iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, 3, total); //<-- Total is output param
    std::cout << iResult << " " << total << std::endl;

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2);
    std::cout << sResult <<  std::endl;

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2,-3);
    std::cout << sResult << std::endl;

    sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, 3, total); //<-- Total is output param
    std::cout << sResult << " " << total << std::endl;

    return 0;
}
```

### Usage

To use this code, you can simply copy and paste it ([source](CodeChallenge.cpp)) into a C++ file and run it in your
favorite C++ compiler.

### Editing the Code

The code contains an editable section that can be modified to implement the new requirement. The editable section is
marked in the code with comments like this:

```c++
//<<<<<<<< START OF EDITABLE SECTION OF CODE <<<<<<<<<<<
// code goes here
//<<<<<<<< END OF EDITABLE SECTION OF CODE <<<<<<<<<<<
```

To implement the new requirement, you need to modify the code inside the editable section. Specifically, you need to
modify the `checkIfPositive` template function to accept an additional integer parameter, which will be used to output
the sum of the integer parameters passed to the function. You can use the existing code in the `checkIfPositive`
function as is, as it is not relevant to this exercise.

Here's an example of how to call the modified `checkIfPositive` function:

```c++
int total = 0;
int result = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, 3, total);
std::cout << "Result: " << result << ", Total: " << total << std::endl;
```

In this example, the `checkIfPositive` function is called with four integer parameters and an additional integer
parameter (`total`) that is used to output the sum of the integer parameters passed to the function.

### Conclusion

In conclusion, the code provided is a C++ program that demonstrates the use of template functions and object-oriented
programming concepts. The code can be modified to implement new requirements by editing the editable section of the code
and leaving the non-editable section of the code as is.