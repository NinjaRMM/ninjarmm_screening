enable_testing()

set(BINARY ${SUBPROJECT}_test)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true *.h *.cpp)
set(SOURCES ${TEST_SOURCES})

include(GoogleTest)
add_executable(${BINARY} ${TEST_SOURCES})
target_link_libraries(${BINARY} PUBLIC ${SUBPROJECT}_lib GTest::gtest)

gtest_discover_tests(${BINARY})


