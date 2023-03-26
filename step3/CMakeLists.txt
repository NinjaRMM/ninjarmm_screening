cmake_minimum_required (VERSION 3.1)

#Version
SET(CMAKE_CXX_STANDARD 17)

#project
project (CodeChallenge)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(CodeChallenge CodeChallenge.cpp)

