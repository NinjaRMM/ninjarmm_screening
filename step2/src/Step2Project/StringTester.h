#pragma once
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

/// <summary>
/// Tester function definition
/// </summary>
bool TesterFunction(const std::string& tested) {
	return tested == "test";
}

/// <summary>
/// Contains the string function
/// </summary>
int ContainsTheString(std::function<int(const std::string&)> testerFunction, std::vector<std::string> theStrings) {

	return std::count_if(std::begin(theStrings), std::end(theStrings), testerFunction);
}