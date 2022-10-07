cmake_minimum_required(VERSION 3.21)

project(step4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME}
    src/CodeReview.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
