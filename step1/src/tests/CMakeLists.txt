include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

FetchContent_GetProperties(googletest)

enable_testing()

add_executable(unittests unittests.cpp)

target_link_libraries(unittests
    PRIVATE GTest::gtest_main
    PRIVATE step1)

include(GoogleTest)

gtest_discover_tests(unittests)
