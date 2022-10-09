#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Job
{
public:
	virtual string getName() = 0;
	virtual string getDescription() = 0;
	virtual int getHours() = 0;
	string DoWork() {
		return "My work involves " + getDescription();
	}
};

class Programmer: public Job
{
public:
	string getName()
	{
		return this->name;
	}

	string getDescription()
	{
		return this->description;
	}

	int getHours()
	{
		return this->hours;
	}

	Programmer(string name_, string description_, int hours_) :name(name_), description(description_), hours(hours_){};

private:
	string name;
	string description;
	int hours;
};

class Pilot : public Job
{

public:
	string getName()
	{
		return this->name;
	}

	string getDescription()
	{
		return this->description;
	}

	int getHours()
	{
		return this->hours;
	}

	Pilot(string name_, string description_, int hours_) :name(name_), description(description_), hours(hours_) {};

private:
	string name;
	string description;
	int hours;
};

const uint32_t INBOUNDS = 0;
const uint32_t OUTBOUNDS = 1;

template <typename T> 
uint32_t IsInBounds(uint32_t code, T min, T max)
{
	uint32_t response = INBOUNDS;
	cout << endl << "  Code: " << code << " is";
	if ((code > max) || (code < min))
	{
		cout << " not";
		response = OUTBOUNDS;
	}
	cout << " in the range: [" << min << ", " << max << "]\n";
	return response;
}

int ContainsTheString(auto functor, vector<string> vec)
{
	int ret = 0;
	for (auto & a : vec)
	{
		if (functor(a))
		{
			ret++;
		}
	}
	return ret;
}

template<typename T>
void getDistance(T i)
{
	cout << i.first << " " << i.second << endl;
}

void part1()
{
	vector<unique_ptr<Job>> jobs_data;
	unique_ptr<Job> document_pr = make_unique<Programmer>("doxygen", "Document a piece of software.", 10);
	unique_ptr<Job> trainning_pilot = make_unique<Pilot>("trainning", "Get familiar with the position.", 40);

	unique_ptr<Job> bugfix_pr = make_unique<Programmer>("bugfix", "Solve problem with API.", 20);
	unique_ptr<Job> travel_pilot = make_unique<Pilot>("travel", "Trip to Paris.", 30);

	unique_ptr<Job> meeting_pr = make_unique<Programmer>("meeting", "Present findings in the project.", 3);
	unique_ptr<Job> meeting_pilot = make_unique<Pilot>("meeting", "Weekly meeting.", 1);

	unique_ptr<Job> supper_pr = make_unique<Programmer>("support", "Help people in the group.", 8);
	unique_ptr<Job> lunch_pilot = make_unique<Pilot>("lunch", "Company lunch with CEO.", 2);

	jobs_data.emplace_back(move(document_pr));
	jobs_data.emplace_back(move(trainning_pilot));
	jobs_data.emplace_back(move(bugfix_pr));
	jobs_data.emplace_back(move(travel_pilot));
	jobs_data.emplace_back(move(meeting_pr));
	jobs_data.emplace_back(move(meeting_pilot));
	jobs_data.emplace_back(move(supper_pr));
	jobs_data.emplace_back(move(lunch_pilot));

	for (auto & a : jobs_data)
	{
		cout << endl
			<< "  Name: " << a->getName() << endl
			<< "  Description: " << a->getDescription() << endl
			<< "  Hours: " << a->getHours() << endl
			<< "  DoWork: " << a->DoWork() << endl;
	}

}

void part2()
{
	/*   Template function */
	uint32_t httpResponseCode = 500;
	IsInBounds<uint32_t>(httpResponseCode, 500, 599);

	httpResponseCode = 200;
	IsInBounds<uint32_t>(httpResponseCode, 500, 599);
}

void part3()
{
    auto theStrings = vector<std::string>{ "one", "two", "test", "test", "jose", "test"};
	auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; } , theStrings);

	cout << endl << "  Number: " << count << endl;
}

void ClientCode()
{
	// Job with classes
	part1();

	// Template
	part2();

	// Lambda
	part3();
}

int main()
{
	ClientCode();
	return 0;
}
