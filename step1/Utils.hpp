#ifndef STEP1_UTILS_HPP
#define STEP1_UTILS_HPP

#include <functional>
#include <string>
#include <vector>

namespace Utils
{
	template <typename T>
	bool IsInBounds(T input, T lowerBound, T upperBound)
	{
		bool success = true;
		if (input < lowerBound || input > upperBound)
		{
			success = false;
		}
		return success;
	}

	template <typename T>
	void BoundsTestPrint(T in, T low, T hi, bool res)
	{
		std::cout <<
			"For values of" << std::endl <<
			"input = " << in << std::endl <<
			"lower bound = " << low << std::endl <<
			"upper bound = " << hi << std::endl <<
			"the result is " << (res ? "true" : "false") << std::endl;
	}

	uint32_t ContainsTheString(const std::function<bool(const std::string)>& test_func, std::vector<std::string> stringList)
	{
		uint32_t count = 0;
		for (auto&& string : stringList)
		{
			count += test_func(string) ? 1 : 0;
		}
		return count;
	}
}

#endif

