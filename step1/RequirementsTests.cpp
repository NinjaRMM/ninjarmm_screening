
#include "pch.h"
#include "Requirements.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>


class RequirementTests : public testing::Test
{
public:
	Requirements m_req{};
};

TEST_F(RequirementTests, DynamicAllocation)
{
	m_req.DynamicAllocation(); // assert does not crash
}

TEST_F(RequirementTests, IsInBounds)
{
	constexpr uint32_t valid_value = 100;
	constexpr uint32_t valid_low = 50;
	constexpr uint32_t valid_high = 150;
	EXPECT_TRUE(IsInBounds(valid_value, valid_low, valid_high));

	constexpr uint32_t too_low_value = 100;
	constexpr uint32_t too_low_low = 200;
	constexpr uint32_t too_low_high = 250;
	EXPECT_FALSE(IsInBounds(too_low_value, too_low_low, too_low_high));

	constexpr uint32_t too_high_value = 100;
	constexpr uint32_t too_high_low = 50;
	constexpr uint32_t too_high_high = 90;
	EXPECT_FALSE(IsInBounds(too_high_value, too_high_low, too_high_high));
}

TEST_F(RequirementTests, ContainsTheString)
{
	const std::vector<std::string> strings{
		"one",
		"two",
		"three",
		"four"
	};

	auto containsOne = [](const std::string& value)
	{
		return value == "one";
	};
	EXPECT_EQ(m_req.ContainsTheString(containsOne, strings), 1);

	auto containsOneAndTwo = [](const std::string& value)
	{
		return value == "one" || value == "two";
	};
	EXPECT_EQ(m_req.ContainsTheString(containsOneAndTwo, strings), 2);

	auto containsNone = [](const std::string& value)
	{
		return false;
	};
	EXPECT_EQ(m_req.ContainsTheString(containsNone, strings), 0);
}

TEST_F(RequirementTests, CSV)
{
	const std::vector<std::vector<std::string>> values {
		{"id", "type", "name"},
		{"0", "bird", "blue"},
		{"1", "cat", "felix"},
		{"2", "dog", "toto"}
	};

	const std::vector<std::vector<std::string>> expected {
		{"id", "0", "1", "2"},
		{"type", "bird", "cat", "dog"},
		{"name", "blue", "felix", "toto"}
	};

	const std::vector<std::vector<std::string>> actual = m_req.Transpose(values);

	EXPECT_EQ(actual, expected);
}