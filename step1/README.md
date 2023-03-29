# NINJARMM C++ Job Program - Step 1

This is a small program in C++ that implements a Job class with two subclasses, Programmer and Pilot. The program
provides an interface for retrieving the name, description, and number of hours required by a job, as well as a function
that prints out the job description.

## Build Requirements

To compile and run this program, you will need:

- A C++ compiler that supports C++17 or later
- CMake version 3.1 or later
- A build tool supported by CMake, such as Make or Ninja

## Source code

The source code for this program is available on GitHub:

[Step1.cpp](./Step1.cpp)

## Usage

To build and run the program, follow these steps:

1. Clone the repository and navigate to the root directory.

   ```bash
       git clone https://github.com/alehorst/ninjarmm_screening.git
       cd ninjarmm_screening/step1
   ```

2. Create a build directory and navigate to it.

   ```bash 
       mkdir -p build; cd build 
   ```

3. Run CMake to configure the build.

    ```bash 
    cmake ..
    ```

4. Build the program using your preferred build tool (e.g. make, ninja).

   ```bash 
   cmake --build . --config Release
   ```

5. Run the program.

   ```bash
   Release/step1
   ```

6. If you have doxygen installed you can build documentation.

   ```bash
   cmake --build . --target doc_doxygen
   ```
   Then open `html/index.html` in your browser.

## Software Requirements

---
**NOTE**

The requirements below are copied from the [original document](Requirements.txt).

---

The program implements the following requirements:

1. Create a class Job with subclasses Programmer and Pilot.
2. The Job class should provide an interface that can:
   - Get the name of the job
   - Get the description of the job
   - Get the number of hours required by the job
   - A DoWork function that prints out "My work involves " + the description.
3. Dynamically allocate each type of job.
4. Store each instance in a collection.
5. Iterate the collection and call each of the available interface functions.
6. Create a template function that will test if the provided values are within a range. Call the function and print out
   the inputs and result. The following should execute assuming httpResonse is a `uint32_t` value:
   - `IsInBounds<uint32_t>(httpResonseCode, 500, 599)`
7. Create a function that will take a vector of strings and a test function, It should return how many items in the
   collection matched the test. Then print out the result. The following should execute assuming theStrings is a vector
   of strings:
   ```c++
   auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
   std::cout << count << " strings matched the test." << std::endl;
   ```
8. C++17 code is preferred, but C++11 is acceptable.

9. Write any other code that demonstrates understanding of C/C++ and programming in general, such as a variadic template
   function.

## Unit Tests

The program includes unit tests for the Job class and utility functions. To run the tests, build the program with
the `UNIT_TESTS` option enabled and run the `teststep1` executable.

```bash
cmake --build . --config Debug --target teststep1 ; ctest -VV -C Debug
```
