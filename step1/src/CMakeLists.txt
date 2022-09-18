cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME CPPQuestions)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#--------------------------------------
if(MSVC)
    if(MSVC_VERSION GREATER_EQUAL 1900)
        # SDL checks 2015+
        add_compile_options(/sdl)
    endif()
else()
    add_compile_options(-Wall -Wextra)
endif()

# Define Release by default
#--------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "Build type not specified: Use Release by default")
endif(NOT CMAKE_BUILD_TYPE)

#--------------------------------------
add_executable(sort
    sort/quickSort.h
    sort/insertionSort.h
    sort/radixSort.h
    sort/main2.cpp
)

#--------------------------------------
add_executable(main4
    main4.cpp
)

#--------------------------------------
add_executable(main5
    main5.cpp
)

#--------------------------------------
add_executable(main5a
    component.h
    main5a.cpp
)

#--------------------------------------
add_executable(main5b
    component.h
    main5b.cpp
)

add_executable(move_forward
    move_forward.cpp
)

add_executable(oopPrinciples
    oopPrinciples.cpp
)

add_executable(iac_versus
    ica_versus.cpp
)

add_executable(shareResources
    shareResources.cpp
)
