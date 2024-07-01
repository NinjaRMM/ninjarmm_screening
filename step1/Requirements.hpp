
#pragma once

#include <iostream>
#include <vector>
#include <string>

class Requirements
{
public:
	void DynamicAllocation();

	size_t ContainsTheString(bool (*func)(const std::string&), const std::vector<std::string>& strings);

	std::vector<std::vector<std::string>> Transpose(const std::vector<std::vector<std::string>>& values);

};

template<class CODE_TYPE>
bool IsInBounds(const CODE_TYPE& value, const CODE_TYPE& low, const CODE_TYPE& high)
{
	std::cout << "IsInBounds value: " << value << " low: " << low << " high: " << high << std::endl;
	return value >= low && value <= high;
}