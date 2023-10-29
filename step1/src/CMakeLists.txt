cmake_minimum_required(VERSION 3.26)

project(step1
    VERSION 1.0
    DESCRIPTION "NinjaOne Screening - Step 1"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)

add_subdirectory(apps)

add_subdirectory(tests)
