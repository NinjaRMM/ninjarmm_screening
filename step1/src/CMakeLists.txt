cmake_minimum_required(VERSION 3.1.0)

project(step1)

set(CMAKE_CXX_STANDARD 17)

add_library(step1lib
    Job.h
    Job.cpp
    Programmer.h
    Programmer.cpp
    Pilot.h
    Pilot.cpp
    Utils.h)

add_executable(step1 main.cpp)
target_link_libraries(step1 step1lib)

add_executable(step1Tests tests.cpp)
target_link_libraries(step1Tests step1lib)

enable_testing()
add_test(NAME step1Tests COMMAND step1Tests)
