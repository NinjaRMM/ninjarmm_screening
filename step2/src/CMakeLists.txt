cmake_minimum_required(VERSION 3.10)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()


# set the project name
project(Step2)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(os)

# add the executable
add_executable(Step2 main.cpp)


target_link_libraries(Step2 PUBLIC os)


