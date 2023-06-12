cmake_minimum_required(VERSION 3.0)
project(step3_program LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Locate and include Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include(CTest)
enable_testing()

# Add source files
set(SOURCES
    src/CodeChallenge.cpp
)

# Add an executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Include the 'inc' directory for header files
target_include_directories(${PROJECT_NAME} PRIVATE inc)

# Set the source files for the test program
set(TEST_SOURCES 
    test/test_CodeChallenge.cpp
)

# Create the test program executable
add_executable(step3_test ${TEST_SOURCES})

# Link the test program with the Google Test libraries
target_link_libraries(step3_test gtest gtest_main pthread)

# Include the 'inc' directory for header files
target_include_directories(step3_test PRIVATE inc)

# Check the operating system
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message("Running on Windows")
    # Add a test that do memory analysis with Intel Inspector
    add_test(NAME Intel_inspector COMMAND inspxe-cl -collect mi1 -- ./${PROJECT_NAME})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Running on Linux")
    # Add a test that do memory analysis with Valgrind
    add_test(NAME Valgrind COMMAND valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./${PROJECT_NAME})
else()
    message("Unknown operating system: ${CMAKE_SYSTEM_NAME}")
endif()
    
# Add a test that runs the test executable with Google Test framework
add_test(NAME Google_test COMMAND ./step3_test)