cmake_minimum_required(VERSION 3.10.0)
project(step4)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    CodeReview.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE kTEST_CodeReview)
if(MSVC_VERSION GREATER_EQUAL 1900)
    # SDL checks 2015+
    target_compile_options(${PROJECT_NAME} PRIVATE /sdl)
endif()
