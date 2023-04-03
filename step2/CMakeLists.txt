cmake_minimum_required(VERSION 3.26)

project(Step2)
set(PROJECT_SOURCES CodeReview.cpp main.cpp)
add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES} CodeReview.h)