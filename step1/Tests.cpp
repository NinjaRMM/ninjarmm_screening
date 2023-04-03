#ifndef STEP1_TESTS_HPP
#define STEP1_TESTS_HPP
#include <iostream>
#include <sstream>
#include <type_traits>
#include <variant>
#include <utility>
#include "Utils.hpp"
#include "Job.hpp"
#include "Pilot.hpp"
#include "Programmer.hpp"

template<typename T>
std::string ToStringImpl(T arg)
{
	std::stringstream ss;
	ss << arg;
	return ss.str();
}

template<typename StrippedArg>
std::vector<std::string> ToStringVec(StrippedArg&& stripped)
{
	std::vector<std::string> outVec;
	outVec.push_back(ToStringImpl(stripped));
	return outVec;
}

template<typename StrippedArg, typename ... ArgTypes>
std::vector<std::string> ToStringVec(StrippedArg&& stripped, ArgTypes&& ... argPack)
{
	size_t packSize = sizeof...(argPack);
	std::vector<std::string> outVec;
	outVec.push_back(ToStringImpl(stripped));
	std::vector<std::string> recurseVec = ToStringVec(argPack ...);
	outVec.insert(outVec.end(), recurseVec.begin(), recurseVec.end());
	return outVec;
}

template<typename ExpectedActual, typename ... ArgTypes>
void PrintError(int&& lineNumber, ExpectedActual&& expected, ExpectedActual&& actual, ArgTypes&& ... args)
{
	std::cout << "Failure at line #:" << lineNumber << std::endl;
	std::cout << "Expected: " << ToStringImpl(expected) << std::endl;
	std::cout << "Got: " << ToStringImpl(actual) << std::endl;
	std::cout << "Inputs: " ;
	std::vector<std::string> inputsVec = ToStringVec(args ...);
	for(auto&& string : inputsVec)
	{
		std::cout << "[" << string << "] ";
	}
	std::cout << std::endl;
}

template<typename ExpectedActual, typename ArgType>
void PrintError(int&& lineNumber, ExpectedActual expected, ExpectedActual actual, ArgType& arg)
{
	std::cout << "Failure at line #:" << lineNumber << std::endl;
	std::cout << "Expected: " << ToStringImpl(expected) << std::endl;
	std::cout << "Got: " << ToStringImpl(actual) << std::endl;
	std::cout << "Input: " << std::endl;
	std::cout << "[" << ToStringImpl(arg) << "] " << std::endl;
}

class TestResults
{
public:
	TestResults() = default;
	~TestResults() = default;
	void tripFailure()
	{
		localTestResults_ = false;
		unitTestResult_ = false;
		globalTestResult_ = false;
	}
	void resetLocalLevel()
	{
		localTestResults_ = false;
	}
	void resetUnitLevel()
	{
		localTestResults_ = false;
		unitTestResult_ = true;
	}
	void resetGlobalLevel()
	{
		localTestResults_ = false;
		unitTestResult_ = true;
		globalTestResult_ = true;
	}
	bool passedLocalLevel()
	{
		return unitTestResult_;
	}
	bool passedUnitLevel()
	{
		return unitTestResult_;
	}
	bool passedGlobalLevel()
	{
		return globalTestResult_;
	};
private:
	bool localTestResults_ = true;
	bool unitTestResult_ = true;
	bool globalTestResult_ = true;
};

int main ()
{
	TestResults testResults;
	std::cout << "Displaying Basic PrintError functionality (This is not an actual error)" << std::endl;
	PrintError(-1, true, false, 53, 42.5, "word", 'l');
	std::cout << std::endl;
	std::cout << "Testing Jobs Compilation" << std::endl;
	{ // This test block will throw compiler errors if invalid
		std::vector<std::shared_ptr<Job>> jobList;
		jobList.push_back(std::shared_ptr<Job>(new Pilot()));
		jobList.push_back(std::shared_ptr<Job>(new Programmer()));
		// Printout per requirements
		for (auto&& job : jobList)
		{
			std::cout << "Name of job: " << job->GetName() << std::endl;
			std::cout << "Description of job: " << job->GetDescription() << std::endl;
			std::cout << "Average weekly hours of job: " << job->GetHours() << std::endl;
			job->DoWork();
		}
	}
	std::cout << std::endl;
	std::cout << "Testing Pilot Output" << std::endl;
	{
		Pilot pilot;
		std::string expectedStr = "Pilot";
		std::cout << "Testing Pilot::GetName()" << std::endl;
		if (pilot.GetName() != expectedStr)
		{
			PrintError(__LINE__, expectedStr, pilot.GetName(), std::string("Pilot::GetName()"));
			testResults.tripFailure();
		}
		std::cout << (testResults.passedLocalLevel() ? "PASSED" : "FAILED") << std::endl;
		testResults.passedLocalLevel();

		std::cout << "Testing Pilot::GetDescription()" << std::endl;
		expectedStr = "Operating and controlling aircraft along planned routes and during takeoffs and landings.\n" \
			"Monitoring engines, fuel consumption, and other aircraft systems during flight.\n" \
			"Responding to changing conditions, such as weather events and emergencies (for example, a mechanical malfunction)";
		if (pilot.GetDescription() != expectedStr)
		{
			PrintError(__LINE__, expectedStr, pilot.GetDescription(), std::string("Pilot::GetDescription()"));
			testResults.tripFailure();
		}
		std::cout << (testResults.passedLocalLevel() ? "PASSED" : "FAILED") << std::endl;
		testResults.passedLocalLevel();

		std::cout << "Testing Pilot::GetHours()" << std::endl;
		uint32_t expectedUint32 = 20;
		if (pilot.GetHours() != expectedUint32)
		{
			PrintError(__LINE__, expectedUint32, pilot.GetHours(), std::string("Pilot::GetHours()"));
			testResults.tripFailure();
		}
		std::cout << (testResults.passedLocalLevel() ? "PASSED" : "FAILED") << std::endl;
		testResults.passedLocalLevel();

		//per requirements, DoWork does not return a value, and cannot be tested
	}
	std::cout << "Pilot tests " << (testResults.passedUnitLevel() ? "passed" : "FAILED") << std::endl;
	testResults.resetUnitLevel();

	std::cout << std::endl;
	std::cout << "Testing Programmer Output" << std::endl;
	{
		Programmer programmer;
		std::string expectedStr = "Programmer";
		std::cout << "Testing Programmer::GetName()" << std::endl;
		if (programmer.GetName() != expectedStr)
		{
			PrintError(__LINE__, expectedStr, programmer.GetName(), std::string("Programmer::GetName()"));
			testResults.tripFailure();
		}
		std::cout << (testResults.passedLocalLevel() ? "PASSED" : "FAILED") << std::endl;
		testResults.passedLocalLevel();

		std::cout << "Testing Programmer::GetDescription()" << std::endl;
		expectedStr = "Writing, modifying, and testing code and scripts that allow computer software and applications to function properly.\n" \
			"Turning the designs created by software developers and engineers into instructions that a computer can follow.";
		if (programmer.GetDescription() != expectedStr)
		{
			PrintError(__LINE__, expectedStr, programmer.GetDescription(), std::string("Programmer::GetDescription()"));
			testResults.tripFailure();
		}
		std::cout << (testResults.passedLocalLevel() ? "PASSED" : "FAILED") << std::endl;
		testResults.passedLocalLevel();

		std::cout << "Testing Programmer::GetHours()" << std::endl;
		uint32_t expectedUint32 = 40;
		if (programmer.GetHours() != expectedUint32)
		{
			PrintError(__LINE__, expectedUint32, programmer.GetHours(), std::string("Programmer::GetHours()"));
			testResults.tripFailure();
		}
		std::cout << (testResults.passedLocalLevel() ? "PASSED" : "FAILED") << std::endl;
		testResults.passedLocalLevel();

		//per requirements, DoWork does not return a value, and cannot be tested
	}
	std::cout << "Programmer tests " << (testResults.passedUnitLevel() ? "passed" : "FAILED") << std::endl;
	std::cout << std::endl;
	testResults.resetUnitLevel();

	std::cout << "Testing Utils::IsInBounds" << std::endl;
	// arbitrary bound points
	const uint32_t LOWER_BOUND = 500; 
	const uint32_t UPPER_BOUND = 600;
    std::cout << "Out of lower bounds test" << std::endl;
	for (uint32_t testVal = 0; testVal < LOWER_BOUND && testResults.passedLocalLevel(); testVal += 1)
	{
		bool expected = false;
		bool individualTestResult = Utils::IsInBounds(testVal, LOWER_BOUND, UPPER_BOUND);
		if (individualTestResult != expected)
		{
			PrintError(__LINE__, expected, individualTestResult, testVal, LOWER_BOUND, UPPER_BOUND);
			testResults.tripFailure();
		}
	}
	std::cout << (testResults.passedLocalLevel() ? "PASSED" : "FAILED") << std::endl;
	
	std::cout << "In bounds test" << std::endl;
	testResults.resetLocalLevel();
	for (uint32_t testVal = LOWER_BOUND; testVal <= UPPER_BOUND && testResults.passedLocalLevel(); testVal += 1)
	{
		bool expected = true;
		bool individualTestResult = Utils::IsInBounds(testVal, LOWER_BOUND, UPPER_BOUND);
		if (individualTestResult != expected)
		{
			PrintError(__LINE__, expected, individualTestResult, testVal, LOWER_BOUND, UPPER_BOUND);
			testResults.tripFailure();
		}
	}
	std::cout << (testResults.passedLocalLevel() ? "PASSED" : "FAILED") << std::endl;
	
	std::cout << "Out of upper bounds test" << std::endl;
	testResults.resetLocalLevel();
	bool rolloverProtectionExitCond = false;
	for (uint32_t testVal = UPPER_BOUND + 1; testVal <= UINT32_MAX && testResults.passedLocalLevel() && !rolloverProtectionExitCond; testVal += 1)
	{
		bool expected = false;
		bool individualTestResult = Utils::IsInBounds(testVal, LOWER_BOUND, UPPER_BOUND);
		if (individualTestResult != expected)
		{
			PrintError(__LINE__, expected, individualTestResult, testVal, LOWER_BOUND, UPPER_BOUND);
			testResults.tripFailure();
		}
		rolloverProtectionExitCond = testVal == UINT32_MAX;
	};
	std::cout << (testResults.passedLocalLevel() ? "PASSED" : "FAILED") << std::endl;

	std::cout << "Utils::IsInBounds tests " << (testResults.passedUnitLevel() ? "passed" : "FAILED") << std::endl;

	std::cout << std::endl << "Testing Utils::ContainsTheString" << std::endl;
	testResults.resetUnitLevel();
	const std::vector<std::string> stringTestVector = // generated by https://www.lipsum.com/
	{
		"Lorem",		"ipsum",		"dolor",		"sit",		"amet",
		"consectetur",	"adipiscing",	"elit",			"Quisque",	"eleifend",
		"dui",			"quis",			"vestibulum",	"commodo",	"Etiam",
		"tincidunt",	"diam",			"nec",			"lobortis",	"commodo",
		"Nam",			"ultricies",	"urna",			"at",		"bibendum",
		"varius",		"Etiam",		"non",			"nulla",	"at",
		"diam",			"rhoncus",		"lacinia"
	};

	std::cout << "Testing that the string vector contains all it's elements" << std::endl;
	for (const auto& testString : stringTestVector)
	{
		const bool expected = true;
		const auto count = Utils::ContainsTheString([testString](const std::string& tested) { return tested == testString; }, stringTestVector);
		const bool individualTestResult = count > 0;
		if (individualTestResult != expected)
		{
			PrintError(__LINE__, expected, individualTestResult, testString);
			testResults.tripFailure();
		}
	}
	std::cout << (testResults.passedLocalLevel() ? "PASSED" : "FAILED") << std::endl;
	
	std::string testString = stringTestVector.at(0);
	std::cout << "Testing that the string \"" << testString << "\" can be found in the string vector" << std::endl;
	testResults.resetLocalLevel();
	{ // test scope
		const bool expected = true;
		const auto count = Utils::ContainsTheString([testString](const std::string& tested) { return tested == testString; }, stringTestVector);
		const bool individualTestResult = (count == 1);
		if (individualTestResult != expected)
		{
			PrintError(__LINE__, expected, individualTestResult, testString);
			testResults.tripFailure();
		}
	}
	std::cout << (testResults.passedLocalLevel() ? "PASSED" : "FAILED") << std::endl;

	testString = "frog";
	std::cout << "Testing that the string \"" << testString << "\" cannot be found in the string vector" << std::endl;
	testResults.resetLocalLevel();
	{ // test scope
		const bool expected = false;
		const auto count = Utils::ContainsTheString([testString](const std::string& tested) { return tested == testString; }, stringTestVector);
		const bool individualTestResult = (count > 0);
		if (individualTestResult != expected)
		{
			PrintError(__LINE__, expected, individualTestResult, testString);
			testResults.tripFailure();
		}
	}
	std::cout << (testResults.passedLocalLevel()  ? "PASSED" : "FAILED") << std::endl;

	testString = stringTestVector.at(13);
	std::cout << "Testing that the 2 instances of the string \"" << testString << "\" can be found in the string vector" << std::endl;
	testResults.resetLocalLevel();
	{ // test scope
		const bool expected = true;
		const auto count = Utils::ContainsTheString([testString](const std::string& tested) { return tested == testString; }, stringTestVector);
		const bool individualTestResult = (count == 2);
		if (individualTestResult != expected)
		{
			PrintError(__LINE__, expected, individualTestResult, testString);
			testResults.tripFailure();
		}
	}
	std::cout << (testResults.passedLocalLevel() ? "PASSED" : "FAILED") << std::endl;

	std::cout << "Utils::ContainsTheString tests " << (testResults.passedUnitLevel() ? "passed" : "FAILED") << std::endl;

	std::cout << std::endl << (testResults.passedGlobalLevel() ? "All tests passed" : "Test FAILURES") << std::endl;

	return testResults.passedGlobalLevel() ? 0 : -1;
}

#endif

