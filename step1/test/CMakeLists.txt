include(FetchContent)

set(TEST_BINARY_NAME ${PROJECT_NAME}Test)

# Import DocTest (for testing) from GitHub at configure time
find_package(Git REQUIRED)

set(DOCTEST_GIT_TAG    v2.4.11)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG        ${DOCTEST_GIT_TAG}
)

FetchContent_MakeAvailable(doctest)
FetchContent_GetProperties(doctest)

# This enables the doctest_discover_tests function so tests can be
# discovered in the UI
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${doctest_SOURCE_DIR}/doctest
)

add_executable(${TEST_BINARY_NAME}
  "src/main.cpp"
  "src/unit-test.cpp"
  "${PROJECT_SOURCE_DIR}/src/test.cpp" # It is also possible to create a library in the parent and link against it
)

doctest_discover_tests(${TEST_BINARY_NAME})
