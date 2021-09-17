// Author: Paul Hatfield
// Contact: paulhat33@gmail.com
/*
Requirements:

1. Write a small program in C++ that will contain the following:
	a. Create a class Job
	b. Create a subclass Programmer
	c. Create a subclass Pilot
	d. The job class should provide an interface that can do the following
		i. Get the name of the job
		ii. Get the description of the job.
		iii. Get the number of hours required by the job
		iv. A DoWork function that prints out “My work involves ” + the description.
	e. Dynamically allocate each type of job.
	f. Store each instance in a collection.
	g. Iterate the collection and call each of the available interface functions.
	h. Create a template function that will test if the provided values are within a 
	   range. Call the function and print out the inputs and result. The following
	   should execute assuming httpResonse is a uint32_t value:
		i. IsInBounds<uint32_t>(httpResonseCode, 500, 599)
	i. Create a function that will take a vector of strings and a test function, It
	   should return how many items in the collection matched the test. Then print out
	   the result.
		i. auto theStrings = vector<string> { “one”, “two”,  “test”};
		ii. auto count = ContainsTheString([](const string& tested) { return tested == “test” }, theStrings);
	j. Write any other code you feel would show off your understanding of C/C++ and 
	   programming in general. Bonus: Variadic Template /w usage.
	k. All The source can be in 1 file, it should compile and there should be no runtime
	   errors, and no memory leaks. Use built in types freely to help you manage resources.
*/
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std; // For readability

// a. // 
class Job
{
public:
	Job() {}
	virtual ~Job() {}

	// i. Get the name of the job
	virtual string name() const = 0;

	// ii. Get the description of the job.
	virtual string description() const = 0;

	// iii. Get the number of hours required by the job
	virtual int reqHours() const = 0;

	// iv. A DoWork function that prints out “My work involves ” + the description.
	virtual void DoWork() const { cout << "My work involves " << description() << endl; }
};

// b. // 
class Programmer : public Job
{
public:
	Programmer() : Job() {}
	virtual ~Programmer() {}

	virtual string name() const override { return "Programmer"; }
	virtual string description() const override { return "writing code."; }
	virtual int reqHours() const override { return 900; }
};

// c. // 
class Pilot : public Job
{
public:
	Pilot() : Job() {}
	virtual ~Pilot() {}

	virtual string name() const override { return "Pilot"; }
	virtual string description() const override { return "flying aircraft."; }
	virtual int reqHours() const override { return 5000; }
};

// h. // 
template <typename T>
bool IsInBounds(T val, T min, T max)
{
	return (val < min ? false : (val > max ? false : true));
}

// i. // 
int ContainsTheString(const std::function<bool(const std::string&)> fn, const vector<string>& list)
{
	int count = 0;
	for_each(list.begin(), list.end(), [&fn, &count](string s) {
		if (fn(s)) { ++count; }
		});
	return count;
}


int main()
{
	// e. f. g. // 
	auto jobTest = [](Job* job) {
		cout << "Name: " << job->name() << endl;
		cout << "Desc: " << job->description() << endl;
		cout << "Hours: " << job->reqHours() << endl;
		job->DoWork();
		cout << endl;
		delete job;
		job = nullptr;
	};

	vector<Job*> collection{ new Programmer, new Pilot };
	for_each(collection.begin(), collection.end(), jobTest);

	// h. //
	bool inBounds = false;
	uint32_t httpResonseCode = 598;

	inBounds = IsInBounds<uint32_t>(httpResonseCode, 500, 599);
	cout << "IsInBounds (" << httpResonseCode << ", " << 500 << ", " << 599
		<< ")  result: " << boolalpha << inBounds << endl;

	// i. // 
	auto theStrings = vector<string>{ "one", "two",  "test" };
	auto count = ContainsTheString([](const string& tested) { return tested == "test"; }, theStrings);
	cout << "ContainsTheString = " << count << endl;

	return 0;
}

