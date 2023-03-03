#include <string>
#include <iostream>
#include <vector>
#include <functional>

class IJob {

public:
	IJob() noexcept {}

	IJob(const std::string& _name, const std::string& _description, const unsigned int& _hours) noexcept {}

	virtual ~IJob() {}

	// Default interface (step1, item d: requirements)
	virtual std::string getName() const = 0;

	virtual std::string getDescription() const = 0;

	virtual unsigned int getHours() const = 0;

	virtual void DoWork() = 0;

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
};

class Programmer : public Job {
public:

	Programmer() noexcept {}

	Programmer(const std::string& _name, const std::string& _description, const unsigned int& _hours) : Job(_name, _description, _hours) { }

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

	virtual ~Pilot() {}

	void DoWork() override {
		std::cout << "I'm a pilot, and... ";
		Job::DoWork();
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
	// Tip: take memory snapshot before entering client code scope
	{
		// Client code... Step 1: itens a, b, c, d, e, f, and g;
		std::cout << "====== Step 1: itens a, b, c, d, e, f, and g =======\n\n";
		auto jobs = std::vector<std::shared_ptr<IJob>>();

		auto pr = std::make_shared<Job>(Programmer{ "Jeremias", "programming in C++", 40 });
		auto pi = std::make_shared<Job>(Pilot{ "Jeremiah", "flying a Boeing", 12 });
		auto jo = std::make_shared<Job>(Job{ "Jerry", "cleaning everything up", 8 });

		jobs.push_back(pr);
		jobs.push_back(pi);
		jobs.push_back(jo);

		for (auto& j : jobs) {
			std::cout << "Hi, my name is " << j->getName() << "\n";
			std::cout << "My job description: " << j->getDescription() << "\n";
			std::cout << "Hours required by the job " << j->getHours() << " h\n";
			j->DoWork();
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
		std::cout << "IsInBounds(" << httpResponse << ", " << lo << ", " << up << ") = " << resStr << "\n\n";
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

		printOutput("Strings vector: [", constOutputStr, "] has ", constCountStr, " ocourrence(s) of \"", constTargetStr, "\" string\n\n");
		std::cout << "----------------------------------------------------\n\n";
	}
	// Tip: take memory another snapshot after leaving client code scope
	// We should not have any memory leaks here, since memory allocation done via smart pointers (RAII)
	return 0;
}