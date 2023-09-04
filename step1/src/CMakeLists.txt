cmake_minimum_required(VERSION 3.22) 

project(screening_step1)

include(FetchContent)
FetchContent_Declare(
     googletest
     URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB step1_SRC
     "main.cpp"
     "functions.h"
     "Job.h"
     "Job.cpp"
     "Pilot.h"
     "Pilot.cpp"
     "Programmer.h"
     "Programmer.cpp"
)

file(GLOB tests_SRC
     "tests.cpp"
     "functions.h"
     "Job.h"
     "Job.cpp"
     "Pilot.h"
     "Pilot.cpp"
     "Programmer.h"
     "Programmer.cpp"
)

add_executable(step1 ${step1_SRC})
add_executable(tests ${tests_SRC})

target_link_libraries(tests PRIVATE gtest)

include(GoogleTest)
gtest_discover_tests(tests)

