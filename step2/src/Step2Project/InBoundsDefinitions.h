#pragma once
#include <cstdint>
#include <iostream>

/// <summary>
/// Is In Bounds Function
/// </summary>
template <typename T>
T IsInBounds(T httpResponseCode, T minimumValue, T maximumValue) {

	std::cout << "HTTP Response Code: " << httpResponseCode << std::endl;
	std::cout << "Minimum Value: " << minimumValue << std::endl;
	std::cout << "Maximum Value: " << maximumValue << std::endl;

	return (httpResponseCode >= minimumValue) && (httpResponseCode <= maximumValue);
}

/// <summary>
/// Test is in bounds
/// </summary>
void TestIsInBounds(uint32_t testValue) {

	if (IsInBounds<uint32_t>(testValue, 500, 599)) 
	{
		std::cout << "Is In Bounds" << std::endl;
	}
	else
	{
		std::cout << "Is Not In Bounds" << std::endl;
	}
}