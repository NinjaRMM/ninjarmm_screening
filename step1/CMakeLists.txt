cmake_minimum_required(VERSION 3.15)
project(step1)

include(CTest)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(test/)
add_executable  (${CMAKE_PROJECT_NAME}
  "src/main.cpp"
  "src/test.cpp"
)
