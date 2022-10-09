cmake_minimum_required(VERSION 3.2)
project(SystemInfo)
set(BIN_NAME "system_info")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

file(GLOB_RECURSE SOURCE RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(CMAKE_CXX_STANDARD 17)

add_executable(
    ${BIN_NAME}
    ${SOURCE}
)

target_link_libraries(
    ${BIN_NAME}
)
