#include <iostream>
#include <memory>
#include <vector>
#include <functional>

#include "Programmer.hpp"
#include "Pilot.hpp"
#include "Utils.hpp"

int main()
{
	std::vector<std::shared_ptr<Job>> jobList;
	jobList.push_back(std::shared_ptr<Job>(new Pilot()));
	jobList.push_back(std::shared_ptr<Job>(new Programmer()));
	for (auto&& job : jobList)
	{
		std::cout << "Name of job: " << job->GetName() << std::endl;
		std::cout << "Description of job: " << job->GetDescription() << std::endl;
		std::cout << "Average weekly hours of job: " << job->GetHours() << std::endl;
		job->DoWork();
	}
	
	uint32_t lowerBound = 500;
	uint32_t upperBound = 599;
	uint32_t httpResponseCode = 502;
	Utils::BoundsTestPrint(httpResponseCode, lowerBound, upperBound, Utils::IsInBounds(httpResponseCode, lowerBound, upperBound));
	httpResponseCode = 499;
	Utils::BoundsTestPrint(httpResponseCode, lowerBound, upperBound, Utils::IsInBounds(httpResponseCode, lowerBound, upperBound));
	httpResponseCode = 600;
	Utils::BoundsTestPrint(httpResponseCode, lowerBound, upperBound, Utils::IsInBounds(httpResponseCode, lowerBound, upperBound));

	auto theStrings = std::vector<std::string>{ "one", "two", "test"};
	auto count = Utils::ContainsTheString([](const std::string& tested){ return tested == "test";}, theStrings);
	std::cout << "count is " << count << std::endl;
	return 0;
}
