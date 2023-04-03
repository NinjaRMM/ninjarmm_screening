Stephen William David Pilcher

For step 1, I set up a simple CMake config
The tests call for a C++17 compliant compiler
A Variadic template function was used for error printing in the unit tests.
The unit tests provide no less information then a standard googletest results (actually provides more information to the console than a normal EXPECT or EXPECT_THAT).

Step 2 needed a header and so many includes

Step 3 also has a CMake config, but does not require C++17
While the implementation of the solution per test statements was trivial, it's very bad practice to add non-optional parameters after a defaulted parameter, and espicially one with an identical type signiture.
The overload with the return value should be in front of the parameter list, or part of the return in a std::pair, but given the requirements, that was not possible. 

Standard procedure for me to configure and build (as I use it)
From inside a Step folder
cmake . -B build
cmake --build build --config Release
you can find the executables in step#/build/Release
