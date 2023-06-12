# NinjaRMM Test Material - Step 3

This repository contains code for a coding challenge in the context of a test material for NinjaRMM. The challenge is divided into multiple steps, and this code is for Step 3.

## Code Files

The following source code files are included in this repository:

1. CodeChallenge.cpp: This file contains the main code for the Step 3 coding challenge. It includes the implementation of the checkIfPositive function template and demonstrates its usage with different types and objects.
2. CodeChallenge.hpp: This header file provides the declarations of the SampleObject1 and SampleObject2 structures, which represent sample objects used in the coding challenge. It also defines the checkIfPositive function template.
3. test_CodeChallenge.cpp: This file contains unit tests for the checkIfPositive function template. It uses the Google Test framework to define and run the tests.
4. CMakeLists.txt: This CMake build configuration file specifies the project structure, dependencies, and build instructions. It sets up the build process for the main program and the unit test program.

## Prerequisites

- C++ compiler
- CMake
- Google Test framework (gtest)
- Valgrind (OS Linux)
- Intel Inspector (OS Windows)

## Building and Running

1. Clone the repository:

```
git clone https://github.com/guilhermeelkadri/ninjarmm_screening.git
```

2. Navigate to the project directory:


```
cd step1
```

3. Create a build directory:

```
mkdir build
cd build
```

4. Generate the build files:

```
cmake ..
```

5. Build the program:

```
cmake --build .
```

6. Run the program:

```
./step1
```

Note: The build instructions assume a Linux. If you are using Windows, adjust the commands accordingly.

## Testing

The code includes unit tests for the checkIfPositive function template. The tests are defined in the test_CodeChallenge.cpp file and use the Google Test framework. You can run the tests by executing the step3_test executable, as mentioned in the "Building and Running" section.

Additionally, there are two memory analysis tests included:

Intel Inspector (Windows): The test can be run using the Intel_inspector target, which performs memory analysis with Intel Inspector. Execute the following command:

Valgrind (Linux): The test can be run using the Valgrind target, which performs memory analysis with Valgrind. Execute the following command:

``` bash
ctest --verbose
```

Please note that the memory analysis tests require the corresponding tools (Intel Inspector or Valgrind) to be installed on your system.
