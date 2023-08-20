enable_testing()

add_executable(
  TestJobs
  TestJobs.cpp
)

target_link_libraries(
  TestJobs
  GTest::gtest_main
)
include(GoogleTest)

install(
  TARGETS
        TestJobs
  DESTINATION 
    ${CMAKE_INSTALL_PREFIX}/step1/
)
