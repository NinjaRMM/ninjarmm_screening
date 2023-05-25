project(NinjaOneCppScreeningStep1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 1)

cmake_minimum_required(VERSION 3.16)

include(FetchContent)
fetchcontent_declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
fetchcontent_makeavailable(googletest)


set(CMAKE_CXX_STANDARD 20)

add_library(libstep1 SHARED
  src/step1/Job.cpp
  src/step1/Pilot.cpp
  src/step1/Programmer.cpp
)

target_include_directories(
  libstep1
  PUBLIC include
)

add_executable(step1 src/Main.cpp)

add_executable(test_step1
  test/step1/JobTest.cpp
  test/step1/PilotTest.cpp
  test/step1/ProgrammerTest.cpp
)

target_link_libraries(step1 libstep1)
target_link_libraries(test_step1 libstep1 gtest_main gmock_main)

include(GoogleTest)
gtest_discover_tests(test_step1)
