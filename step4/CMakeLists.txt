cmake_minimum_required(VERSION 3.2)
project(CodeReview)
set(BIN_NAME "code_review")

file(
    GLOB_RECURSE SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/ScopedCoInitialize.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/ScopedCoInitialize.hpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/ThirdPartyAVSoftware.hpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/CodeReview.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/CodeReview.hpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    ${BIN_NAME}
    ${SOURCE}
)

target_link_libraries(
    ${BIN_NAME}
)
