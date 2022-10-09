cmake_minimum_required(VERSION 3.2)
project(CheckIfPositive)
set(BIN_NAME "checkIfPositive")

file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/CodeChallenge.cpp")

set(CMAKE_CXX_STANDARD 17)

add_executable(
    ${BIN_NAME}
    ${SOURCE}
)

target_link_libraries(
    ${BIN_NAME}
)
