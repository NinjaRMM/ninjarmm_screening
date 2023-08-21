enable_testing()

add_executable(
  TestJobs
  TestJobs.cpp
)

target_link_libraries(
  TestJobs
  GTest::gtest_main
)

add_executable(
  TestTemplatedBounds
  TestTemplatedBounds.cpp
)

target_link_libraries(
  TestTemplatedBounds
  GTest::gtest_main
)

add_executable(
  TestStringCount
  TestStringCount.cpp
)

target_link_libraries(
  TestStringCount
  GTest::gtest_main
)

include(GoogleTest)

install(
    TARGETS
        TestJobs
        TestTemplatedBounds
        TestStringCount
  DESTINATION 
    ${CMAKE_INSTALL_PREFIX}/step1/
)
