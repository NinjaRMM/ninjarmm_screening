# NinjaRMM Test Material - Step 1

This is a test program for the NinjaRMM test material, specifically designed for Step 1.

## Description

The program demonstrates the usage of polymorphism and template functions. It includes two classes, `Programmer` and `Pilot`, both derived from the abstract base class `Job`. The `Job` class defines four pure virtual functions: `action1()`, `action2()`, `action3()`, and `action4()`. The derived classes `Programmer` and `Pilot` implement these functions.

The program also includes two template functions: `IsInBounds()` and `ContainsTheString()`. `IsInBounds()` checks if a value is within a specified range, while `ContainsTheString()` counts the number of strings in a collection that satisfy a given condition.

The program includes a `main()` function that creates instances of `Programmer` and `Pilot`, stores them in a vector, and calls the virtual functions for each job. It also demonstrates the usage of the template functions.

## Prerequisites

- C++ compiler
- CMake
- Google Test framework (gtest)
- Valgrind (OS Linux)
- Intel Inspector (OS Windows)

## Build and Run

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

7. Run the tests:

```
ctest --verbose
```