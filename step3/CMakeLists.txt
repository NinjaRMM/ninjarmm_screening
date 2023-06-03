cmake_minimum_required(VERSION 3.15)

project(Step3 VERSION 1.0 LANGUAGES CXX)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_11)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_executable(Step3 CodeChallenge.cpp)

target_link_libraries(Step3 compiler_flags)

# TESTING
enable_testing()

add_test(NAME SimpleTest COMMAND Step3)
set_tests_properties(SimpleTest PROPERTIES
  WILL_FAIL FALSE
)

add_test(NAME TestOutput COMMAND Step3)
set_tests_properties(TestOutput PROPERTIES
  PASS_REGULAR_EXPRESSION "1 6.+true 6"
)
