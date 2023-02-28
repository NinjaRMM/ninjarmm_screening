#include "CppUnitTest.h"
#include <string>
#include <list>
#include <iostream>
#include <map>
#include <functional>
#include <stdint.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class Job {
public:
	/* Getter for all class members. */
	auto name() const& -> const std::string& { return name_; }
	auto description() const& -> const std::string& { return description_; }
	auto reqHours() const& -> const int& { return reqHours_; }

	auto DoWork() const& -> const int& { std::cout << "My work involves " << this->description_ << std::endl; return 1; }

	Job(std::string name, std::string description, int reqHours)
		: name_(name), description_(description), reqHours_(reqHours) {};

protected:
	std::string name_;
	std::string description_;
	int reqHours_;
};

class Pilot :public Job {
public:
	Pilot(std::string name, std::string description, int reqHours) : Job::Job(name, description, reqHours) {};
};

class Programmer :public Job {
public:
	Programmer(std::string name, std::string description, int reqHours) : Job::Job(name, description, reqHours) {};
};

class JobService {
public:
	template<typename T>
	void addNewJob(std::string name, std::string description, unsigned int reqHours) {
		T job(name, description, reqHours);
		internalJobList.insert({ getNextId(), job });
	}
	template<typename T>
	void addManyJobs(T jobs...) {
		T job(name, description, reqHours);
		internalJobList.insert({ getNextId(), jobs });
		addManyJobs(jobs)
	}

	void removeJob(unsigned int id) {
		if (getNextId() > id) {
			internalJobList.erase(id);
		}
	}

	void printAllJobs() {
		for (auto& job : getJobList()) {
			std::cout
				<< "Job number: #    " << job.first << std::endl
				<< "Job name:        " << job.second.name() << std::endl
				<< "Job description: " << job.second.description() << std::endl
				<< "Job hours:       " << job.second.reqHours() << std::endl;
			job.second.DoWork();
			std::cout << std::endl;
		}
	}

	auto getJobList() const& -> const std::map<unsigned int, Job>& { return internalJobList; }

	~JobService() { internalJobList.clear(); }

private:
	std::map<unsigned int, Job> internalJobList;

	unsigned int getNextId() {
		unsigned int listSize = (unsigned int)internalJobList.size();
		return ++listSize;
	}
};

class RemoteService {
public:
	template <class type>
	bool IsInBounds(type httpResponseCode, type min, type max) {
		bool result = (httpResponseCode >= min) && (httpResponseCode <= max);

		std::string confirmationMessage = result ?
			"Received httpResponse is in bounds:" : "Received httpResponse not in bounds:";

		std::cout << confirmationMessage << std::endl
			<< "Received httpResponse: " << httpResponseCode << std::endl
			<< "Minimum required: " << min << std::endl
			<< "Maximum required: " << max << std::endl
			<< std::endl;

		return result;
	}
};

class StringUtils {
public:
	int ContainsTheString(std::function<bool(std::string& testInputString)> func, std::vector<std::string> testStringVector) {
		int count = 0;
		for (std::string& str : testStringVector) {
			if (func(str)) { count++; }
		}

		std::cout << "Total strings matching with the specified function: " << count << std::endl;
		return count;
	}
};

int main()
{
	JobService jobService;
	jobService.addNewJob<Pilot>("Linus", "teste", 8);
	jobService.addNewJob<Pilot>("Charlie", "teste", 8);
	jobService.printAllJobs();

	RemoteService rs;
	rs.IsInBounds<uint32_t>(530, 500, 599);

	auto theStrings = std::vector<std::string>{ "one", "two", "test" };
	StringUtils strUtils;
	auto count = strUtils.ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);

	return 0;
}

namespace Step1Test
{
    TEST_CLASS(UnitTest1)
    {
    public:
		TEST_METHOD(TestJobServiceConstructor)
		{
			JobService jobService;
			jobService.addNewJob<Pilot>("Linus", "teste", 8);
			jobService.addNewJob<Pilot>("Charlie", "teste", 8);

			Assert::IsTrue(jobService.getJobList().size() == 2);
			Assert::IsTrue(jobService.getJobList().at(1).name() == "Linus");
			Assert::IsTrue(jobService.getJobList().at(1).description() == "teste");
			Assert::IsTrue(jobService.getJobList().at(1).reqHours() == 8);
			Assert::IsTrue(jobService.getJobList().at(2).name() == "Charlie");
			Assert::IsTrue(jobService.getJobList().at(2).description() == "teste");
			Assert::IsTrue(jobService.getJobList().at(2).reqHours() == 8);
		}

		TEST_METHOD(TestRemoteServiceResponseInBounds)
		{
			RemoteService rs;
			bool result = rs.IsInBounds<uint32_t>(530, 500, 599);

			Assert::IsTrue(result);
		}

		TEST_METHOD(TestRemoteServiceResponseNotInBounds)
		{
			RemoteService rs;
			bool result = rs.IsInBounds<uint32_t>(600, 500, 599);

			Assert::IsFalse(result);
		}

		TEST_METHOD(TestVectorContainString)
		{
			auto theStrings = std::vector<std::string>{ "one", "two", "test" };
			StringUtils strUtils;
			auto count = strUtils.ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);

			Assert::IsTrue(count == 1);
		}

		TEST_METHOD(TestVectorDontContainString)
		{
			auto theStrings = std::vector<std::string>{ "one", "two", "not_test" };
			StringUtils strUtils;
			auto count = strUtils.ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);

			Assert::IsTrue(count == 0);
		}
    };
}