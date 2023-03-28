cmake_minimum_required(VERSION 3.12) 

project(step1) 

add_subdirectory(../../googletest step1) 

include_directories(../../googletest/include)

add_executable(step1Tests step1Tests.cpp) 

target_link_libraries(step1Tests PRIVATE gtest gtest_main) 