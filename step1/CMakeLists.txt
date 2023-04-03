cmake_minimum_required(VERSION 3.26)
project(Step1)
set(PROJECT_SOURCES main.cpp Programmer.cpp Pilot.cpp)
set(PROJECT_HEADERS Job.hpp Programmer.hpp Pilot.hpp Utils.hpp)
add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})
target_sources(${CMAKE_PROJECT_NAME} PRIVATE FILE_SET HEADERS FILES ${PROJECT_HEADERS})

set(PROJECT_TEST_SOURCES Tests.cpp Programmer.cpp Pilot.cpp)
set(PROJECT_TEST_HEADERS Job.hpp Programmer.hpp Pilot.hpp Utils.hpp)
add_executable(${CMAKE_PROJECT_NAME}_Tests ${PROJECT_TEST_SOURCES})
target_sources(${CMAKE_PROJECT_NAME}_Tests PRIVATE FILE_SET HEADERS FILES ${PROJECT_TEST_HEADERS})
target_compile_features(${CMAKE_PROJECT_NAME}_Tests PRIVATE cxx_std_17)

