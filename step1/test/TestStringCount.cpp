#include <gtest/gtest.h>
#include "../src/StringCount.hpp" 

TEST(TemplatedBounds, CorrectlyCountsStringByUsingALambdaAsTestingFunction) {

  const auto theStrings = std::vector<std::string> { "one", "two",  "test"};
  const size_t result = ContainsTheString([](const std::string& tested) { return tested == "test";}, theStrings);

  EXPECT_EQ(1, result);
}
