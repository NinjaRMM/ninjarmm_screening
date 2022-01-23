
#include <iostream>
#include <iostream>
#include <string> 
#include <vector>
#include <functional>
using namespace std;


	//a.Create a class Job
	//	b.Create a subclass Programmer
	//	c.Create a subclass Pilot
	//	d.The job class should provide an interface that can do the following
	//	i.Get the name of the job
	//	ii.Get the description of the job.
	//	iii.Get the number of hours required by the job
	//	iv.A DoWork function that prints out “My work involves ” + the description.
	//	e.Dynamically allocate each type of job.
	//	f.Store each instance in a collection.
	//	g.Iterate the collection and call each of the available interface functions.
	//	h.Create a template function that will test if the provided values are within a range.Call the function and print out the inputs and result.The following should execute assuming httpResonse is a uint32_t value :
	//i.IsInBounds<uint32_t>(httpResonseCode, 500, 599)

	//	i.Create a function that will take a vector of strings and a test function, It should return how many items in the collection matched the test.Then print out the result.
	//	i. auto theStrings = vector<std::string>{ “one”, “two”,  “test” };
	//ii. auto count = ContainsTheString([](const std::string& tested) { return tested == “test” }, theStrings);

	//j.Write any other code you feel would show off your understanding of C / C++ and programming in general.Bonus: Variadic Template / w usage.
	//	k.All The source can be in 1 file, it should compile and there should be no runtime errors, and no memory leaks.Use built in types freely to help you manage resources.
// a)
class Job
{
protected:
	string jobName = "";
	string jobDescription = "";
	int jobHours = 0;
public:
	// d)
	string GetJobName()
	{
		return jobName;
	}
	string GetJobDescription()
	{
		return jobDescription;
	}
	int GetJotHours()
	{
		return jobHours;
	}
	void DoWork()
	{
		cout << "My work involes:  " << jobDescription << std::endl;
	}

};

// b)
class Programmer :public Job
{

public:
	Programmer(string name, string description, int hours)
	{
		jobName = name;
		jobDescription = description;
		jobHours = hours;
	}

	~Programmer()
	{

	}
};


// c)
class Pilot:public Job
{

public:
	Pilot(string name, string description, int hours)
	{
		jobName = name;
		jobDescription = description;
		jobHours = hours;
	}

	~Pilot()
	{

	}

};


// h)	//	h.Create a template function that will test if the provided values are within a range.
// Call the function and print out the inputs and result.The following should execute assuming httpResonse is a uint32_t value :
//i.IsInBounds<uint32_t>(httpResonseCode, 500, 599)

template <typename T>
bool IsInBounds(T responseCode, T min, T max) {
	bool isInBound = false;
	if ((responseCode >= min)
		&& (responseCode <= max))
	{
		isInBound = true;
	}
	return isInBound;
}

// i) 	i.Create a function that will take a vector of strings and a test function, It should return how many items in the collection matched the test.Then print out the result.
	//	i. auto theStrings = vector<std::string>{ “one”, “two”,  “test” };
	//ii. auto count = ContainsTheString([](const std::string& tested) { return tested == “test” }, theStrings);

int ContainsTheString(bool (*f)(const::string&), vector<std::string> strings)
{
	int count = 0;
	
	for (auto &s : strings)
	{
		if (f(s) )
		{
			count += 1;
		}
	}

	return count;
}


//j.Write any other code you feel would show off your understanding of C / C++ and programming in general.
// Bonus: Variadic Template / w usage.
void f() {}

template<typename T>
void g(T x)
{
	std::cout << x << ","<< endl;
}

template<typename T, typename... Tail>
void f(T head, Tail...tail)
{
	g(head);
	f(tail...);
}




int main()
{
	std::cout << "Step2" << std::endl;

	// e)
	Programmer progDavid("David", "C++ Programmer", 10);
	Programmer ProgEd("Ed", "C# Programer", 20);

	Pilot pilotAnn("Ann", "Private Pilot", 10);
	Pilot pilotBob("Bob", "Jet Pilot", 20);
	Pilot pilotCat("Cat", "Commercial Pilot", 30);
	// f)
	vector<std::unique_ptr<Job>> jobList;
	jobList.push_back(std::make_unique<Programmer>(progDavid));
	jobList.push_back(std::make_unique<Programmer>(ProgEd));
	jobList.push_back(std::make_unique<Pilot>(pilotAnn));
	jobList.push_back(std::make_unique<Pilot>(pilotBob));
	jobList.push_back(std::make_unique<Pilot>(pilotCat));

	// g)
	std::cout << "g)" << endl;
	for (auto &job : jobList)
	{
		job->DoWork();
	}

	// h)
	std::cout << "h)" << endl;
	uint32_t httpResonseCode = 502;
	bool isInBound = IsInBounds<uint32_t>(httpResonseCode, 500, 599);
	cout << "Input: " << httpResonseCode << std::endl;
	cout << "Is in Bound : " << isInBound << std::endl;


	//  i)
	std::cout << "i)" << endl;
	auto theStrings = vector<std::string> { "one", "two", "test" };
	auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
	std::cout << "Count: " << count << std::endl;
	

	// j)
	std::cout << "j) Variadic Template" << endl;
	f(1, 2, "three");

	std::cout << "Variadic Template 2" << endl;
	f(1, 2, "three", 4, "Five");
}

// Output: Step2
//Step2
//g)
//My work involes : C++ Programmer
//My work involes : C# Programer
//My work involes : Private Pilot
//My work involes : Jet Pilot
//My work involes : Commercial Pilot
//h)
//Input: 502
//	Is in Bound : 1
//	i)
//	Count: 1
//	j) Variadic Template
//	1,
//	2,
//	three,
//	Variadic Template 2
//	1,
//	2,
//	three,
//	4,
//	Five,
