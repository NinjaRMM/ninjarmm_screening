
cmake_minimum_required(VERSION 3.25)

project( step3 
         VERSION 0.1
         DESCRIPTION "Ninjarmm screenging - step3"
         LANGUAGES CXX)

add_executable(step3 CodeChallenge.cpp)

target_compile_features(step3 PRIVATE cxx_std_20)

