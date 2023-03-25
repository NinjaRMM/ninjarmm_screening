cmake_minimum_required (VERSION 3.1)

#Version
SET(CMAKE_CXX_STANDARD 17)

# Third Party
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
###############################################

#project
project (Step1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(Step1 Step1.cpp)

#tests
enable_testing()
add_executable(Step1Tests "${CMAKE_CURRENT_SOURCE_DIR}/Step1Tests.cpp")

target_link_libraries(Step1Tests 
	PRIVATE 
		GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Step1Tests)
