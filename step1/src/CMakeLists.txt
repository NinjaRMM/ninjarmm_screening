cmake_minimum_required(VERSION 3.5)
project(my_project)

# Add the executable target
add_executable(step1 step1.cpp)

# Link the executable with the cppunit library
target_link_libraries(step1 cppunit)