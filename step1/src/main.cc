// g++ -Wall -Wextra -pedantic -std=c++11 main.cc && ./a.out

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

template <typename T>
void printTestMessage(T message)
{
	std::cout << message;
}

template <typename T, typename... Args>
void printTestMessage(T message, Args... args)
{
	std::cout << message << " ";
	printTestMessage(args...);
}

template <typename... Args>
void printTestMessage()
{
	std::cout << std::endl;
}

template <typename... Args>
void test(bool condition, Args... args)
{
	if (!condition) {
		std::cout << "Test failed: ";
		printTestMessage(args...);
		std::cout << std::endl;
		exit(1);
	}
}

class Job
{
public:
	virtual std::string getJobName() = 0;
	virtual std::string getJobDescription() = 0;
	virtual int getRequiredHours() = 0;
	void doWork()
	{
		std::cout << "My work involves " << this->getJobName() << std::endl;
	}
};

class Programmer : public Job
{
public:
	std::string getJobName()
	{
		return "Programmer";
	}

	std::string getJobDescription()
	{
		return "I write code";
	}

	int getRequiredHours()
	{
		return 40;
	}
};

class Pilot : public Job
{
public:
	std::string getJobName()
	{
		return "Pilot";
	}

	std::string getJobDescription()
	{
		return "I fly planes";
	}

	int getRequiredHours()
	{
		return 60;
	}
};

template <typename T>
bool isInBounds(T value, T lower, T upper)
{
	return (value >= lower && value <= upper);
}

bool containsTheString(std::function<bool(const std::string &)> func, std::vector<std::string> strings)
{
	for (auto &str : strings)
		if (func(str))
			return true;
	return false;
}

void jobsTask()
{
	std::vector<std::unique_ptr<Job>> jobs;
	jobs.push_back(std::unique_ptr<Programmer>(new Programmer()));
	jobs.push_back(std::unique_ptr<Pilot>(new Pilot()));

	for (auto &job : jobs) {
		std::cout << "---" << std::endl;
		std::cout << "Job name: " << job->getJobName() << std::endl;
		std::cout << "Job despcription: " << job->getJobDescription() << std::endl;
		std::cout << "Hours required: " << job->getRequiredHours() << " hours per week" << std::endl;
		job->doWork();
	}
}

void testIsInBounds()
{
	// positive test
	test(isInBounds<uint32_t>(5, 0, 10), 5,  "is not in bounds");
	test(isInBounds<float>(5.0, 0.0, 10.0), 5.0, "is not in bounds");

	// negative test
	test(!isInBounds<uint32_t>(-5, 0, 10), -5, "is in bounds");
	test(!isInBounds<float>(-5.0, 0.0, 10.0), -5.0, "is in bounds");

	// edge test
	test(isInBounds<uint32_t>(0, 0, 10), 0, "is not in bounds");
	test(isInBounds<float>(0.0, 0.0, 10.0), 0.0, "is not in bounds");
	test(isInBounds<uint32_t>(10, 0, 10), 10, "is not in bounds");
	test(isInBounds<float>(10.0, 0.0, 10.0), 10.0, "is not in bounds");

	// specific test
	uint32_t httpResponseCode = 500;
	test(isInBounds<uint32_t>(httpResponseCode, 500, 599), "httpResponseCode is in bounds");

	std::cout << "isInBounds tests passed" << std::endl;
}

void testContainsTheString()
{
	std::vector<std::string> theStrings = { "one", "two", "test" };

	// positive test
	test(containsTheString([](const std::string &tested) { return tested == "one"; }, theStrings),
      			       "one is not in strings");

	// negative test
	test(!containsTheString([](const std::string &tested) { return tested == "three"; }, theStrings),
      				"three is in strings");

	// specific test
	test(containsTheString([](const std::string &tested) { return tested == "test"; }, theStrings),
      			       "test is not in strings");

	std::cout << "containsTheString tests passed" << std::endl;
}

int main()
{
	jobsTask();
	std::cout << "---" << std::endl;

	testIsInBounds();
	std::cout << "---" << std::endl;

	testContainsTheString();
	std::cout << "---" << std::endl;

	std::cout << "All tests passed" << std::endl;
	return 0;
}
