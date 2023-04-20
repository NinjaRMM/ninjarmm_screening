/*
* Step1_i.cpp
* This file covers step 'i' of 'Requirements.txt'
*/

#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>

int ContainsTheString(std::function<bool(std::string)> f, std::vector<std::string> v)
{
	int count = 0;
	for (std::string var:v)
	{
		if (f(var))
		{
			count++;
		}
	}
	return count;
}

int main()
{
	auto theStrings = std::vector<std::string>{ "one", "two", "test"};

	auto count = ContainsTheString(
		[](const std::string& tested) -> bool
		{
			return tested == "test";
		}, theStrings
	);

	std::cout << count << ((count == 1) ? " element" : " elements") << " matched the test" << std::endl;

	return 0;
}