cmake_minimum_required(VERSION 3.26)

project(step3
    VERSION 1.0
    DESCRIPTION "NinjaOne Screening - Step 3"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(step3 CodeChallenge.cpp)

target_compile_features(step3 PRIVATE cxx_std_20)
