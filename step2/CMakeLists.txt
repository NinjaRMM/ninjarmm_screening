cmake_minimum_required(VERSION 3.16)

project(NinjaOneCppScreeningStep2)

set(WINN32 true)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(step2 CodeReview.cpp)
