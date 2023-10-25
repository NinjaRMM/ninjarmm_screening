
cmake_minimum_required(VERSION 3.25)

project( step1 
         VERSION 0.1
         DESCRIPTION "Ninjarmm screenging - step1"
         LANGUAGES CXX)

add_executable(step1 main.cpp)

target_compile_features(step1 PRIVATE cxx_std_20)

