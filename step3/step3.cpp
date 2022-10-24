#include <string>
#include <vector>
#include <iostream>

class Job
{
public:
	std::string jobName_;
	std::string jobDescription_;
	int jobHours_;

	Job()
	{
		jobName_ = "";
		jobDescription_ = "";
		jobHours_ = 0;
	}

	Job(std::string JobName, std::string JobDescription, int JobHours)
	{
		jobName_ = JobName;
		jobDescription_ = JobDescription;
		jobHours_ = JobHours;
	}

	void SetJobName(std::string JobName)
	{
		jobName_ = JobName;
	}

	void SetJobDescription(std::string JobDescription)
	{
		jobDescription_ = JobDescription;
	}

	void SetJobHours(int JobHours)
	{
		jobHours_ = JobHours;
	}

	void DoWork()
	{
		std::cout << "My work involves " << jobHours_ << "hours of " << jobDescription_ << std::endl;
	}
};

class Programmer : public Job
{
public:
	std::string ProgrammerDescription_ = "coding all day everyday!";
	std::string ProgrammerName_ = "Programmer";
	int ProgrammerHours = 10;

	Programmer() : Job()
	{
	}

	Programmer(std::string JobName, std::string JobDescription, int JobHours) : Job()
	{
		this->jobDescription_ = ProgrammerDescription_;
		this->jobName_ = ProgrammerName_;
		this->jobHours_ = ProgrammerHours;
	}
};

class Pilot : public Job
{
public:
	std::string PilotDescription_ = "flying all day everyday!";
	std::string PilotName_ = "PIlot";
	int PilotHours = 8;

	Pilot() : Job()
	{
	}

	Pilot(std::string JobName, std::string JobDescription, int JobHours) : Job()
	{
		this->jobDescription_ = PilotDescription_;
		this->jobName_ = PilotName_;
		this->jobHours_ = PilotHours;
	}
};

template <typename T>
void IsInBounds(T responseCode, T lowerBound, T upperBound)
{
	bool isInBounds = false;

	std::cout << "IsInBounds Call: input = " << responseCode << " lowerBound = " << lowerBound << " upperBound = " << upperBound << std::endl;
	if (responseCode >= lowerBound && responseCode <= upperBound)
	{
		isInBounds = true;
	}
	std::string sIsInBounds = isInBounds ? "true" : "false";
	std::cout << "IsInBounds Result: " << sIsInBounds << std::endl;
}

int ContainsTheString(bool(*func)(const std::string&), std::vector<std::string> strings)
{
	int count = 0;
	for (std::vector<std::string>::iterator it = strings.begin(); it != strings.end(); it++)
	{
		if (func(*it))
		{
			count++;
		}
	}
	return count;
}

int main()
{
	std::vector<Job*> jobList;
	Programmer* programmerJob = new Programmer("Programmer", "coding all day everyday!", 10);
	Pilot* pilotJob = new Pilot("Pilot", "flying all day everyday!", 8);
	jobList.push_back(programmerJob);
	jobList.push_back(pilotJob);

	for (size_t i = 0; i < jobList.size(); i++)
	{
		jobList[i]->DoWork();
	}

	IsInBounds<uint32_t>(1, 500, 599);
	IsInBounds<uint32_t>(501, 500, 599);

	auto theStrings = std::vector<std::string>{ "one", "two", "test"};
	auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
	std::cout << "ContainsTheString count = " << count << std::endl;

	return 0;
}

