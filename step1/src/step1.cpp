#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <functional>

//Memory leak detection
#include "windows.h"
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>  
#include <crtdbg.h>  

class IJob : public std::enable_shared_from_this<IJob> {
private:
	std::weak_ptr<IJob> parent;

public:

	IJob() noexcept {}

	IJob(const std::string& _name, const std::string& _description, const unsigned int& _hours) noexcept {}

	IJob(const IJob&) noexcept {}

	IJob(IJob&&) noexcept {}

	IJob& operator=(const IJob&) = delete;

	IJob& operator=(const IJob&&) = delete;

	virtual ~IJob() {}

	// Default interface (step1 requirements)
	virtual std::string getName() const = 0;

	virtual std::string getDescription() const = 0;

	virtual unsigned int getHours() const = 0;

	virtual void DoWork() = 0;

	// Composite pattern
	virtual void setParent(std::weak_ptr<IJob> _parent) noexcept {
		this->parent = _parent;
	}

	virtual std::weak_ptr<IJob> getParent() const noexcept {
		return this->parent;
	}

	virtual bool isSupervisor() const noexcept {
		return false;
	}

	virtual bool addSupervisedJob(std::shared_ptr<IJob> _component) = 0;

	virtual bool removeSupervisedJob(std::shared_ptr<IJob> _component) = 0;

};

class Job : public IJob {
private:
	std::string name;
	std::string description;
	unsigned int hours;

public:

	Job() noexcept {
		hours = 0;
	}

	Job(const std::string& _name, const std::string& _description, const unsigned int& _hours) :
		name{ _name }, description{ _description }, hours{ _hours } {	}

	Job(const Job&) = delete;

	Job(Job&&) = delete;

	Job& operator=(const Job&) = delete;

	Job& operator=(const Job&&) = delete;

	virtual ~Job() {}

	std::string getName() const override {
		return name;
	}

	std::string getDescription() const override {
		return description;
	}

	unsigned int getHours() const noexcept override {
		return hours;
	}

	void DoWork() override {
		std::cout << "My work involves " << description << '\n';
	}

	bool addSupervisedJob(std::shared_ptr<IJob> _component) override {
		return false;
	}

	bool removeSupervisedJob(std::shared_ptr<IJob> _component) override {
		return false;
	}

};

class Programmer : public Job {
public:

	Programmer() noexcept {}

	Programmer(const std::string& _name, const std::string& _description, const unsigned int& _hours) : Job(_name, _description, _hours) { }

	Programmer(const Programmer&) = delete;

	Programmer(Programmer&&) = delete;

	Programmer& operator=(const Programmer&) = delete;

	Programmer& operator=(const Programmer&&) = delete;

	virtual ~Programmer() {}

	void DoWork() override {
		std::cout << "I'm a programmer, and... ";
		Job::DoWork();
	}
};

class Pilot : public Job {
public:

	Pilot(const std::string& _name, const std::string& _description, const unsigned int& _hours) : Job(_name, _description, _hours) {
	}

	Pilot(const Pilot&) = delete;

	Pilot(Pilot&&) = delete;

	Pilot& operator=(const Pilot&) = delete;

	Pilot& operator=(const Pilot&&) = delete;

	virtual ~Pilot() {}

	void DoWork() override {
		std::cout << "I'm a pilot, and... ";
		Job::DoWork();
	}
};

class Supervisor : public Job {
	std::list<std::shared_ptr<IJob>> supervisedJobs;

public:

	Supervisor() noexcept {}

	Supervisor(const std::string& _name, const std::string& _description, const unsigned int& _hours) : Job(_name, _description, _hours) { }

	Supervisor(const Supervisor&) = delete;

	Supervisor(Supervisor&&) = delete;

	Supervisor& operator=(const Supervisor&) = delete;

	Supervisor& operator=(const Supervisor&&) = delete;

	virtual ~Supervisor() {}

	void DoWork() override {
		Job::DoWork();

		const auto childenIsNotEmpty = !supervisedJobs.empty();

		if (childenIsNotEmpty) {

			std::cout << "And I supervise the following people:\n";

			std::string output = "";

			for (const auto& j : supervisedJobs) {
				if (!output.empty()) {
					output += "\n";
				}
				output += j->getName();
			}

			std::cout << output;
		}
	}

	bool addSupervisedJob(std::shared_ptr<IJob> _component) override {
		if (_component != nullptr) {
			this->supervisedJobs.push_back(_component);
			_component->setParent(shared_from_this());
			return true;
		}
		return false;
	}

	bool removeSupervisedJob(std::shared_ptr<IJob> _component) override {
		if (_component != nullptr) {
			this->supervisedJobs.remove(_component);
			return true;
		}
		return false;
	}

	bool isSupervisor() const noexcept override {
		return true;
	}
};

// Extra "Factory" pattern to help us creating jobs
class JobFactory {
public:
	enum class JobType { programmer = 0, pilot, supervisor, standardJob };

	static std::shared_ptr<IJob> createJob(const JobType& type, const std::string& _name, const std::string& _description, const unsigned int& _hours) {
		std::shared_ptr<IJob> job;
		switch (type) {
			case JobType::programmer: {
				job = std::make_shared<Programmer>(_name, _description, _hours);
				break;
			}
			case JobType::pilot: {
				job = std::make_shared<Pilot>(_name, _description, _hours);
				break;
			}
			case JobType::supervisor: {
				job = std::make_shared<Supervisor>(_name, _description, _hours);
				break;
			}
			default: {
				job = std::make_shared<Job>(_name, _description, _hours);
				break;
			}
		}
		return job;
	}
};

//===========================================================================

template<typename T>
bool IsInBounds(T value, T lowerBound, T upperBound) noexcept {
	return lowerBound <= value && value <= upperBound;
}

//===========================================================================

unsigned int ContainsTheString(std::function<int(const std::string&)> func, const std::vector<std::string>& strings) {
	unsigned int counter = 0;
	for (auto& i : strings) {
		const bool isEqual = func(i);
		if (isEqual) counter++;
	}
	return counter;
}

template<class... Args>
void printOutput(const Args&... args) {
	for (auto& s : { args... }) {
		std::cout << s;
	}
}

void outputHelper(const std::vector<std::string>& stringList, std::string* output) {
	if (output != nullptr) {
		for (auto& s : stringList) {
			if (!(*output).empty()) {
				*output += ", ";
			}
			*output += s;
		}
	}
}


int main() {
	// Taking memory snapshot before entering client code scope
	_CrtMemState sOld;
	_CrtMemState sNew;
	_CrtMemState sDiff;
	_CrtMemCheckpoint(&sOld);
	{
		// Client code... Step 1: itens a, b, c, d, e, f, and g;
		std::cout << "====== Step 1: itens a, b, c, d, e, f, and g =======\n\n";
		auto jobs = std::vector<std::shared_ptr<IJob>>();
		jobs.reserve(4);

		auto pr = JobFactory::createJob(JobFactory::JobType::programmer, "Jeremias", "programming in C++", 40);
		auto pi = JobFactory::createJob(JobFactory::JobType::pilot, "Jeremiah", "flying a Boeing", 12);
		auto jo = JobFactory::createJob(JobFactory::JobType::standardJob, "Jerry", "cleaning everything up", 8);
		auto su = JobFactory::createJob(JobFactory::JobType::supervisor, "Josh", "supervising everyone", 60);

		jobs.push_back(pr);
		jobs.push_back(pi);
		jobs.push_back(jo);
		jobs.push_back(su);

		jobs.at(3)->addSupervisedJob(jobs.at(0));
		jobs.at(3)->addSupervisedJob(jobs.at(1));
		jobs.at(3)->addSupervisedJob(jobs.at(2));

		for (auto& j : jobs) {
			std::cout << "- Hi, my name is " << j->getName() << "\n";
			std::cout << "- My job description: " << j->getDescription() << "\n";
			std::cout << "- Hours required by the job " << j->getHours() << " h\n- ";
			j->DoWork();
			auto s = j->getParent().lock();
			if (s != nullptr) {
				std::cout << "- My supervisor is: " << s->getName() << "\n";
			}
			std::cout << "\n";
		}
		std::cout << "----------------------------------------------------\n\n\n";

		// Client code... Step 1: item h;
		std::cout << "================= Step 1: item h; ==================\n\n";
		auto constexpr httpResponse = std::uint32_t{ 501 };
		auto constexpr lo = std::uint32_t{ 500 };
		auto constexpr up = std::uint32_t{ 599 };

		const auto res = IsInBounds(httpResponse, lo, up);
		auto resStr = res ? "true" : "false";
		std::cout << "IsInBounds(" << httpResponse << ", " << lo << ", " << up << ") = " << resStr << "\n";
		std::cout << "----------------------------------------------------\n\n\n";
		
		// Client code... Step 1: item i;
		std::cout << "================= Step 1: item i; ==================\n\n";
		const auto targetString = std::string("test");
		const auto theStrings = std::vector<std::string>{ "one", "two", "test" };

		const auto count = ContainsTheString(
			[targetString](const std::string& tested) noexcept {
				return tested == targetString;
			},
			theStrings
		);

		auto output = std::string("");
		outputHelper(theStrings, &output);

		auto constOutputStr = output.c_str();
		auto countStr = std::string(std::to_string(count));
		auto constCountStr = countStr.c_str();
		auto constTargetStr = targetString.c_str();

		printOutput("Strings vector: [", constOutputStr, "] has ", constCountStr, " ocourrence(s)\nof \"", constTargetStr, "\" string\n");
		std::cout << "----------------------------------------------------\n\n";
	}
	// Taking memory another snapshot after leaving client code scope
	// We should not have any memory leaks here, since memory allocation done via smart pointers (RAII)
	_CrtMemCheckpoint(&sNew);
	if (_CrtMemDifference(&sDiff, &sOld, &sNew))
	{
		OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
		_CrtMemDumpStatistics(&sDiff);
		OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
		_CrtMemDumpAllObjectsSince(&sOld);
		OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
		_CrtDumpMemoryLeaks();
	}
	else {
		OutputDebugString(L"----------- NO MEMORY LEAKS FOUND ---------\n");
	}
	return 0;
}