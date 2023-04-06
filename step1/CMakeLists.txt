cmake_minimum_required(VERSION 3.1)

#TESTS param allows to compile the unit tests (OFF by default).
#To enable the unit test: cmake -DTESTS=ON ..; make; ctest
option(TESTS "Run unit tests" OFF)
set(PROJECT ninja_one)

if(TESTS)
    set(PROJECT ${PROJECT}_tests)
endif()

project(${PROJECT})

set(SOURCE_DIR ${PROJECT_SOURCE_DIR})

set(SOURCES 
    ${SOURCE_DIR}/src/Pilot.cpp
    ${SOURCE_DIR}/src/Programmer.cpp
)

set(TEST_SOURCES 
    ${SOURCE_DIR}/tests/test.cpp
    ${SOURCE_DIR}/tests/UtilsTest.cpp
    ${SOURCE_DIR}/tests/ProgrammerTest.cpp
)

if(TESTS)
    enable_testing()
    add_executable(
        ${PROJECT_NAME}
        ${SOURCES}
        ${TEST_SOURCES}
    )

    target_link_libraries(${PROJECT_NAME} gtest)

    add_test(
        NAME IsInBoundsTest
        COMMAND ${PROJECT_NAME} --gtest_filter=IsInBoundsTest.*
    )

    add_test(
        NAME ContainsTheStringTest
        COMMAND ${PROJECT_NAME} --gtest_filter=ContainsTheStringTest.*
    )

    add_test(
        NAME ProgrammerTest
        COMMAND ${PROJECT_NAME} --gtest_filter=ProgrammerTest.*
    )


else()
    add_executable(
        ${PROJECT_NAME}
        ${SOURCE_DIR}/src/main.cpp
        ${SOURCES}
    )
endif()

target_include_directories(${PROJECT_NAME} 
    PRIVATE
    "${SOURCE_DIR}/src"
)

target_compile_features(${PROJECT_NAME}  PRIVATE cxx_std_14)

set_target_properties(${PROJECT_NAME}  PROPERTIES 
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
