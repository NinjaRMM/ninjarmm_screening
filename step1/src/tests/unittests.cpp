#include <step1/utils.hpp>

#include <gtest/gtest.h>

TEST(IsInBoundsTest, Integers)
{
    EXPECT_FALSE(IsInBounds<uint32_t>(450, 500, 599));
    EXPECT_TRUE(IsInBounds<uint32_t>(500, 500, 599));
    EXPECT_TRUE(IsInBounds<uint32_t>(550, 500, 599));
    EXPECT_FALSE(IsInBounds<uint32_t>(650, 500, 599));
    EXPECT_TRUE(IsInBounds<uint32_t>(550, 599, 500));
    EXPECT_FALSE(IsInBounds<uint32_t>(650, 599, 500));

    EXPECT_FALSE(IsInBounds<uint16_t>(450, 500, 599));
    EXPECT_TRUE(IsInBounds<uint16_t>(500, 500, 599));
    EXPECT_TRUE(IsInBounds<uint16_t>(550, 500, 599));
    EXPECT_FALSE(IsInBounds<uint16_t>(650, 500, 599));
    EXPECT_TRUE(IsInBounds<uint16_t>(550, 599, 500));
    EXPECT_FALSE(IsInBounds<uint16_t>(650, 599, 500));

    EXPECT_FALSE(IsInBounds<int>(-50, -10, 45));
    EXPECT_TRUE(IsInBounds<int>(-5, -10, -1));
    EXPECT_TRUE(IsInBounds<int>(2, -10, 45));
    EXPECT_FALSE(IsInBounds<int>(0, -10, -1));
    EXPECT_TRUE(IsInBounds<int>(2, 45, -10));
    EXPECT_FALSE(IsInBounds<int>(0, -1, -10));
}

TEST(IsInBoundsTest, Floats)
{
    EXPECT_TRUE(IsInBounds<float>(450.0f, -500.0f, 599.0f));
    EXPECT_FALSE(IsInBounds<float>(-500.0f, 500.0f, 599.0f));
    EXPECT_FALSE(IsInBounds<float>(550.0f, 500.0f, -599.0f));
    EXPECT_FALSE(IsInBounds<float>(650.0f, 500.0f, 599.0f));
    EXPECT_TRUE(IsInBounds<float>(550.0f, 599.0f, 500.0f));
    EXPECT_FALSE(IsInBounds<float>(650.0f, 599.0f, 500.0f));

    EXPECT_TRUE(IsInBounds<double>(450.0, -500.0, 599.0));
    EXPECT_FALSE(IsInBounds<double>(-500.0, 500.0, 599.0));
    EXPECT_FALSE(IsInBounds<double>(550.0, 500.0, -599.0));
    EXPECT_FALSE(IsInBounds<double>(650.0, 500.0, 599.0));
    EXPECT_TRUE(IsInBounds<double>(550.0, 599.0, 500.0));
    EXPECT_FALSE(IsInBounds<double>(650.0, 599.0, 500.0));
}

TEST(IsInBoundsTest, Chars)
{
    EXPECT_TRUE(IsInBounds('a', '0', 'z'));
    EXPECT_TRUE(IsInBounds('0', '0', '0'));
    EXPECT_FALSE(IsInBounds('A', 'a', 'z'));
    EXPECT_FALSE(IsInBounds('a', 'A', 'Z'));
    EXPECT_TRUE(IsInBounds('g', 'a', 'z'));
    EXPECT_FALSE(IsInBounds('a', 'z', 'g'));
}

TEST(IsInBoundsTest, RangePolicies)
{
    EXPECT_FALSE(IsInBounds<uint32_t>(500, 500, 599, RangePolicy::OPEN));
    EXPECT_FALSE(IsInBounds<uint32_t>(500, 500, 599, RangePolicy::LEFT_OPEN));
    EXPECT_TRUE(IsInBounds<uint32_t>(500, 500, 599, RangePolicy::RIGHT_OPEN));
    EXPECT_TRUE(IsInBounds<uint32_t>(500, 500, 599, RangePolicy::CLOSED));

    EXPECT_FALSE(IsInBounds<uint32_t>(599, 500, 599, RangePolicy::OPEN));
    EXPECT_TRUE(IsInBounds<uint32_t>(599, 500, 599, RangePolicy::LEFT_OPEN));
    EXPECT_FALSE(IsInBounds<uint32_t>(599, 500, 599, RangePolicy::RIGHT_OPEN));
    EXPECT_TRUE(IsInBounds<uint32_t>(599, 500, 599, RangePolicy::CLOSED));

    EXPECT_FALSE(IsInBounds<uint32_t>(500, 599, 500, RangePolicy::OPEN));
    EXPECT_FALSE(IsInBounds<uint32_t>(500, 599, 500, RangePolicy::LEFT_OPEN));
    EXPECT_TRUE(IsInBounds<uint32_t>(500, 599, 500, RangePolicy::RIGHT_OPEN));
    EXPECT_TRUE(IsInBounds<uint32_t>(500, 599, 500, RangePolicy::CLOSED));

    EXPECT_FALSE(IsInBounds<uint32_t>(599, 599, 500, RangePolicy::OPEN));
    EXPECT_TRUE(IsInBounds<uint32_t>(599, 599, 500, RangePolicy::LEFT_OPEN));
    EXPECT_FALSE(IsInBounds<uint32_t>(599, 599, 500, RangePolicy::RIGHT_OPEN));
    EXPECT_TRUE(IsInBounds<uint32_t>(599, 599, 500, RangePolicy::CLOSED));

    EXPECT_FALSE(IsInBounds<float>(500.0f, 500.0f, 599.0f, RangePolicy::OPEN));
    EXPECT_FALSE(IsInBounds<float>(500.0f, 500.0f, 599.0f, RangePolicy::LEFT_OPEN));
    EXPECT_TRUE(IsInBounds<float>(500.0f, 500.0f, 599.0f, RangePolicy::RIGHT_OPEN));
    EXPECT_TRUE(IsInBounds<float>(500.0f, 500.0f, 599.0f, RangePolicy::CLOSED));

    EXPECT_FALSE(IsInBounds<float>(599.0f, 500.0f, 599.0f, RangePolicy::OPEN));
    EXPECT_TRUE(IsInBounds<float>(599.0f, 500.0f, 599.0f, RangePolicy::LEFT_OPEN));
    EXPECT_FALSE(IsInBounds<float>(599.0f, 500.0f, 599.0f, RangePolicy::RIGHT_OPEN));
    EXPECT_TRUE(IsInBounds<float>(599.0f, 500.0f, 599.0f, RangePolicy::CLOSED));

    EXPECT_FALSE(IsInBounds<float>(500.0f, 599.0f, 500.0f, RangePolicy::OPEN));
    EXPECT_FALSE(IsInBounds<float>(500.0f, 599.0f, 500.0f, RangePolicy::LEFT_OPEN));
    EXPECT_TRUE(IsInBounds<float>(500.0f, 599.0f, 500.0f, RangePolicy::RIGHT_OPEN));
    EXPECT_TRUE(IsInBounds<float>(500.0f, 599.0f, 500.0f, RangePolicy::CLOSED));

    EXPECT_FALSE(IsInBounds<float>(599.0f, 599.0f, 500.0f, RangePolicy::OPEN));
    EXPECT_TRUE(IsInBounds<float>(599.0f, 599.0f, 500.0f, RangePolicy::LEFT_OPEN));
    EXPECT_FALSE(IsInBounds<float>(599.0f, 599.0f, 500.0f, RangePolicy::RIGHT_OPEN));
    EXPECT_TRUE(IsInBounds<float>(599.0f, 599.0f, 500.0f, RangePolicy::CLOSED));

    EXPECT_FALSE(IsInBounds<char>('A', 'A', 'Z', RangePolicy::OPEN));
    EXPECT_FALSE(IsInBounds<char>('A', 'A', 'Z', RangePolicy::LEFT_OPEN));
    EXPECT_TRUE(IsInBounds<char>('A', 'A', 'Z', RangePolicy::RIGHT_OPEN));
    EXPECT_TRUE(IsInBounds<char>('A', 'A', 'Z', RangePolicy::CLOSED));

    EXPECT_FALSE(IsInBounds<char>('Z', 'A', 'Z', RangePolicy::OPEN));
    EXPECT_TRUE(IsInBounds<char>('Z', 'A', 'Z', RangePolicy::LEFT_OPEN));
    EXPECT_FALSE(IsInBounds<char>('Z', 'A', 'Z', RangePolicy::RIGHT_OPEN));
    EXPECT_TRUE(IsInBounds<char>('Z', 'A', 'Z', RangePolicy::CLOSED));

    EXPECT_FALSE(IsInBounds<char>('A', 'Z', 'A', RangePolicy::OPEN));
    EXPECT_FALSE(IsInBounds<char>('A', 'Z', 'A', RangePolicy::LEFT_OPEN));
    EXPECT_TRUE(IsInBounds<char>('A', 'Z', 'A', RangePolicy::RIGHT_OPEN));
    EXPECT_TRUE(IsInBounds<char>('A', 'Z', 'A', RangePolicy::CLOSED));

    EXPECT_FALSE(IsInBounds<char>('Z', 'Z', 'A', RangePolicy::OPEN));
    EXPECT_TRUE(IsInBounds<char>('Z', 'Z', 'A', RangePolicy::LEFT_OPEN));
    EXPECT_FALSE(IsInBounds<char>('Z', 'Z', 'A', RangePolicy::RIGHT_OPEN));
    EXPECT_TRUE(IsInBounds<char>('Z', 'Z', 'A', RangePolicy::CLOSED));
}

TEST(ContainsTheStringTest, VectorVariation)
{
    EXPECT_EQ(0, ContainsTheString([](const std::string& tested) { return tested == "test"; }, {"one", "two", "three"}));
    EXPECT_EQ(1, ContainsTheString([](const std::string& tested) { return tested == "test"; }, {"test", "two", "three"}));
    EXPECT_EQ(1, ContainsTheString([](const std::string& tested) { return tested == "test"; }, {"one", "test", "three"}));
    EXPECT_EQ(1, ContainsTheString([](const std::string& tested) { return tested == "test"; }, {"one", "two", "test"}));
    EXPECT_EQ(2, ContainsTheString([](const std::string& tested) { return tested == "test"; }, {"test", "two", "test"}));
    EXPECT_EQ(3, ContainsTheString([](const std::string& tested) { return tested == "test"; }, {"test", "test", "test"}));
    EXPECT_EQ(3, ContainsTheString([](const std::string& tested) { return tested == "test"; }, {"one", "two", "test", "test1", "three", "test", "test"}));
    EXPECT_EQ(0, ContainsTheString([](const std::string& tested) { return tested == "test"; }, {}));
}

TEST(ContainsTheStringTest, FunctionVariation)
{
    EXPECT_EQ(1, ContainsTheString([](const std::string& tested) { return tested == "one"; }, {"one", "two", "three"}));
    EXPECT_EQ(1, ContainsTheString([](const std::string& tested) { return tested == "two"; }, {"test", "two", "three"}));
    EXPECT_EQ(1, ContainsTheString([](const std::string& tested) { return tested == "three"; }, {"one", "test", "three"}));
    EXPECT_EQ(1, ContainsTheString([](const std::string& tested) { return tested == "test1"; }, {"one", "two", "test", "test1", "three", "test", "test"}));
    EXPECT_EQ(0, ContainsTheString([](const std::string& tested) { return tested == ""; }, {"one", "two", "test", "test1", "three", "test", "test"}));
    EXPECT_EQ(0, ContainsTheString([](const std::string& tested) { return tested == "one"; }, {}));
    EXPECT_EQ(0, ContainsTheString([](const std::string& tested) { return tested == ""; }, {}));
}

TEST(CountIfTest, VectorVariation)
{
    EXPECT_EQ(0, CountIf([](const std::string& tested) { return tested == "test"; }, "one", "two", "three"));
    EXPECT_EQ(1, CountIf([](const std::string& tested) { return tested == "test"; }, "test", "two", "three"));
    EXPECT_EQ(1, CountIf([](const std::string& tested) { return tested == "test"; }, "one", "test", "three"));
    EXPECT_EQ(1, CountIf([](const std::string& tested) { return tested == "test"; }, "one", "two", "test"));
    EXPECT_EQ(2, CountIf([](const std::string& tested) { return tested == "test"; }, "test", "two", "test"));
    EXPECT_EQ(3, CountIf([](const std::string& tested) { return tested == "test"; }, "test", "test", "test"));
    EXPECT_EQ(3, CountIf([](const std::string& tested) { return tested == "test"; }, "one", "two", "test", "test1", "three", "test", "test"));
    EXPECT_EQ(0, CountIf([](const std::string& tested) { return tested == "test"; }));
}

TEST(CountIfTest, FunctionVariation)
{
    EXPECT_EQ(1, CountIf([](const std::string& tested) { return tested == "one"; }, "one", "two", "three"));
    EXPECT_EQ(1, CountIf([](const std::string& tested) { return tested == "two"; }, "test", "two", "three"));
    EXPECT_EQ(1, CountIf([](const std::string& tested) { return tested == "three"; }, "one", "test", "three"));
    EXPECT_EQ(1, CountIf([](const std::string& tested) { return tested == "test1"; }, "one", "two", "test", "test1", "three", "test", "test"));
    EXPECT_EQ(0, CountIf([](const std::string& tested) { return tested == ""; }, "one", "two", "test", "test1", "three", "test", "test"));
    EXPECT_EQ(0, CountIf([](const std::string& tested) { return tested == "one"; }));
    EXPECT_EQ(0, CountIf([](const std::string& tested) { return tested == ""; }));
}
