cmake_minimum_required(VERSION 3.21)
project(step3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME}
    src/Main.cpp
    src/asked/Asked.cpp
    src/asked/Asked.hpp
    src/extra/Extra.hpp
    src/mpl/Monotonic.cpp
    src/mpl/Polymorphic.cpp
    src/mpl/Polymorphic.hpp
    src/oop/Job.hpp
    src/oop/Pilot.cpp
    src/oop/Pilot.hpp
    src/oop/Programmer.cpp
    src/oop/Programmer.hpp
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
