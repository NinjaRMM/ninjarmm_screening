# NinjaRMM Screening - Ruben Mardones

## Introduction

Some apps and utilities implemented for ninjaOne evaluation

## Pre-requisites

* Install CMake
* Install C++ compiler that supports at least standard C++17

## Build

First of all, it is necessary to initialize the git submodules before starting the build process.

* Googletest : https://github.com/google/googletest.git (used for unit testing).

To initialize git submodules:

```
git submodule update --init 

```

Then, you can build with cmake:

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

As the result of the build, three exec files are generated in folder build/out:

* ninjarmm_screening_step1: app for step1
* ninjarmm_screening_step1_test: app to run the unit tests for step1
* ninjarmm_screening_step3: app for step3

If you build it in Windows, exec files are generated in folder build/out/Release, another exec is generated:

* ninjarmm_screening_step2: app for step2

## Extra Notes

### Step 1 
* Within this step you can check how I organize the code, separating production code from testing code. 
Source code is place under src folder, and a library and an executable are built from the code contained in src. 
The test folder contains the tests, library from production code is linked together with gtest library to build the app
that runs the unit tests. 
* An extra variadic template function is implemented in Utils.h, as it was considered as a bonus. 
* Tested (build and run) both in Linux and Windows

### Step 2
* Blocks with code review are identified in the code with next format:
```
  /* Start Code review block
     Author: Ruben Mardones
     Comment: Reason for change
     Action: Suggested code to be added/changed
  */
  // Code added/changed
  /* End Code review block */
  ```
* Tested (build and run) in Windows, as it contains code specific only for Windows

### Step 3
* Tested (build and run) both in Linux and Windows

