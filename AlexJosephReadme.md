# About
This is summary of code submission for Alex Joseph

# Makefiles
Makefile has been added at each subdirectory level with a higher level Makefile acting as the main driver.

## Targets

**all:** builds all subdirectories with a directory dedicated for object files (build/obj) and binary files (build/bin) in each subdirectory.

**run:** Executes the appropriate binary file for each sub directory.

**test:** Executes unit test in step1 directory. 

There are no unit tests for other directories. Unit test is based on ["Google test"](https://github.com/google/googletest/blob/main/googletest/README.md). So "Google test" need to be installed in the system.

# Build, run and test outputs

- Sample build log is available in *logs/build_out.txt.*

- Sample execution log is available in *logs/run_out.txt.*

- Sample unit test log is available in *logs/test_out.txt.*

# Memory leak report

Valgrind memory leak report is available in *step1/valgrind-report.txt.*

# Demo/Show off code

Demo code for following features are available in *step1/src/DemoCode.h*, *step1/src/DemoCode.cpp*

**Features covered**

- std::unique_ptr supporting custom memory freeing function.

- Variadic templates.