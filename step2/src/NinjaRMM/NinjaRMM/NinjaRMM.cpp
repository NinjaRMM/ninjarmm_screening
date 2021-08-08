#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

class Job
{
public:
	virtual std::string GetName() = 0;
	virtual std::string GetDescription() = 0;
	virtual int GetHours() = 0;
	virtual void DoWork() = 0;
};

class Programmer : public Job
{
public:
	virtual std::string GetName()
	{
		return "Programmer";
	}
	virtual std::string GetDescription()
	{
		return "Code";
	}
	virtual int GetHours()
	{
		return 100;
	}
	virtual void DoWork()
	{
		std::cout << "My work involves " + GetDescription();
	}

};

class Pilot : public Job
{
public:
	virtual std::string GetName()
	{
		return "Pilot";
	}
	virtual std::string GetDescription()
	{
		return "Fly";
	}
	virtual int GetHours()
	{
		return 1;
	}
	virtual void DoWork()
	{
		std::cout << "My work involves " + GetDescription();
	}

};

template <typename T, typename U>
bool IsInBounds (T t, U lower, U upper)
{
	return t >= lower && t <= upper;
}

int ContainsTheString(std::function<int(const std::string&)> aFunction, std::vector<std::string> strings)
{
	return std::count_if(std::begin(strings), std::end(strings), aFunction);
}

int main()
{
	{
	Job* programmer = new Programmer();
	Job* pilot = new Pilot();
	std::vector<Job*> jobs;
	jobs.push_back(programmer);
	jobs.push_back(pilot);
	for (Job* job : jobs)
	{
		std::cout << job->GetName() << std::endl << job->GetDescription() << std::endl << job->GetHours() << std::endl;
		job->DoWork();
		std::cout << std::endl;
	}
	delete programmer;
	delete pilot;
	}

	uint32_t httpResonseCode = 550;
	std::cout << IsInBounds(httpResonseCode, 500, 599) << std::endl;

	auto theStrings = std::vector<std::string>{ "one", "two",  "test" };
	auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
	std::cout << count << std::endl;



}
