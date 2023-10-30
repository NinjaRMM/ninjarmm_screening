CMAKE_MINIMUM_REQUIRED(VERSION 3.27)
project(step1)

add_executable(step1 main.cpp)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# TESTING

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(mainTests main-tests.cpp)

target_link_libraries(mainTests GTest::gtest_main)