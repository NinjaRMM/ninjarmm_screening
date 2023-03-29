cmake_minimum_required(VERSION 3.10)
project(step2)
set(CMAKE_CXX_STANDARD 17)

include_directories(.)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    message("Sorry, this project does not support compiling on ${CMAKE_SYSTEM_NAME}.")
    message(FATAL_ERROR "Please switch to a supported platform.")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
    include_directories("C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.17763.0\\um\\")
    include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/shared/")
    message("Sorry, this project does not support compiling on MinGW.")
    message(FATAL_ERROR "Please switch to a supported platform.")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "CYGWIN" AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_COMPILER /usr/bin/gcc.exe)
    include_directories("/cygdrive/c/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/um")
    include_directories("/cygdrive/c/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/shared")
    message("Sorry, this project does not support compiling on Cygwin.")
    message(FATAL_ERROR "Please switch to a supported platform.")
endif ()

add_executable(CodeReview CodeReview.cpp)
target_link_libraries(CodeReview wscapi)
