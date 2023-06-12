cmake_minimum_required(VERSION 3.20)
project(step1)

set(CMAKE_CXX_STANDARD 20)

add_executable(step1 src/job.hpp src/pilot.hpp src/programmer.hpp src/main.cpp)
