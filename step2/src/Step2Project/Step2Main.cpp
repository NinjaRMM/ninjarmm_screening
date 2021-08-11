#include <iostream>
#include <list>
#include <vector>

#include "Pilot.h"
#include "Programmer.h"
#include "Job.h"

#include "VariadicFunctions.h"
#include "InBoundsDefinitions.h"
#include "StringTester.h"

/// <summary>
/// Main entry
/// </summary>
int main() {
	std::cout << "Main Program" << std::endl;

	Pilot *pilot = new Pilot();
	Programmer *programmer = new Programmer();

	std::list<Job *> jobs;

	jobs.push_back(pilot);
	jobs.push_back(programmer);

	for (std::list<Job*>::const_iterator it = jobs.begin(); it != jobs.end(); ++it) {
		std::cout << "Job Name: " << (*it)->Name() << std::endl;
		std::cout << "Job Description: " << (*it)->Description() << std::endl;
		std::cout << "Job Hours: " << (*it)->Hours() << std::endl;
		(*it)->DoWork();

		std::cout << std::endl;

		delete* it;
	}

	jobs.clear();

	TestIsInBounds(512);

	std::cout << std::endl;

	TestIsInBounds(700);

	std::cout << std::endl;

	auto theStrings = std::vector<std::string>{ "one", "two",  "test" };
	auto count = ContainsTheString(&TesterFunction, theStrings);

	std::cout << "Count of strings: "<< count << std::endl;

	std::cout << std::endl;

	auto theStrings2 = std::vector<std::string>{ "one", "test",  "test" };
	count = ContainsTheString(&TesterFunction, theStrings2);

	std::cout << "Count of strings: " << count << std::endl;

	std::cout << std::endl;

	std::cout << "Variadric Sum: " << sum(1, 2, 3, 4, 5, 6, 7) << std::endl;
	std::cout << "Variadric Multiplier: " << multiply(1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8) << std::endl;

	std::cout << std::endl;

	std::cout << "Variadric Sum: " << sum(1) << std::endl;
	std::cout << "Variadric Multiplier: " << multiply(1.2, 2) << std::endl;
}
