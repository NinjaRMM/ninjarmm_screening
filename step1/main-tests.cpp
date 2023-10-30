#include <gtest/gtest.h>
#include "header.hpp"

TEST(IsInBoundsTest, Integers)
{
  EXPECT_TRUE(IsInBounds<uint32_t>(500, 0, 1000));
  EXPECT_FALSE(IsInBounds<uint32_t>(1000, 0, 500));
  EXPECT_FALSE(IsInBounds<uint32_t>(200, 0, 100));
}

TEST(IsInBoundsTest, Strings)
{
  auto comp = [](const std::string& tested) { return tested == "test"; };

  EXPECT_EQ(2, ContainsTheString(comp, { "one", "two", "test", "test" }));
  EXPECT_EQ(1, ContainsTheString(comp, { "one", "two", "test" }));
  EXPECT_EQ(0, ContainsTheString(comp, { "one", "two" }));
  EXPECT_EQ(0, ContainsTheString(comp, {  }));

  auto comp2 = [](const std::string& tested) { return tested == ""; };
  EXPECT_EQ(0, ContainsTheString(comp2, { "one", "two", "test" }));
  EXPECT_EQ(1, ContainsTheString(comp2, { "" }));
  EXPECT_EQ(0, ContainsTheString(comp2, { }));
}