cmake_minimum_required(VERSION 3.10.0)
project(step3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    step3.cpp
)

if(MSVC)
    if(MSVC_VERSION GREATER_EQUAL 1900)
        # SDL checks 2015+
        target_compile_options(${PROJECT_NAME} PRIVATE /sdl)
    endif()
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()
