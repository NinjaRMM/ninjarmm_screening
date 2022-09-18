cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME step2)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    if(MSVC_VERSION GREATER_EQUAL 1900)
        # SDL checks 2015+
        add_compile_options(/sdl /ZI)
        add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    endif()
else()
    add_compile_options(-Wall -Wextra)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/info.h
    ${CMAKE_CURRENT_SOURCE_DIR}/info_windows.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/info_linux.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/info_macos.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/info_common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/info_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC})

add_executable(step2
    ${SRC}
)
