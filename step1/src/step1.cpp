#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <format>
#include <array>
#include <algorithm>
#include <cassert>

using namespace std;


//The requirement says "provide an interface"
//	01. This could be an interface with only pure virutal functions like Microsoft's COM Interface.
//  OR
//  02. This could be just an interface but can have other member variables, functions such as invariant functions, private functions and so on.

//Implementing with method 02.
class Job
{

public:
	Job(const string& name_, const string& description_, uint32_t requiredHours_) : name(name_), description(description_), requiredHours(requiredHours_)
	{

	}

	//Since virtual dtor, I need to add all other default operations Core Guideline C.21/Rule of five/six
	virtual ~Job() = default;

	Job(Job&&) = default;
	Job& operator=(Job&&) = default;

	Job(const Job&) = default;
	Job& operator=(const Job&) = default;

	//Even though it looks like inline, the compiler will outline it and enter its address in vtbl.
	//Reasonable default implementation for the interface: Subclasses would inherit default implementation and interface
	virtual const string& GetName()
	{
		return name;
	}

	virtual const string& GetDescription()
	{
		return description;
	}

	virtual uint32_t GetRequiredHours()
	{

		return requiredHours;
	}

	//Not much of anything this function can do for subclasses
	virtual void DoWork() = 0;

protected:
	//This method is just to show invariant over specilaization, otherwise simply the subclasses can print this text.
	const string& GetPrefix()
	{

		return prefix;
	}

private:
	string name;
	string description;
	uint32_t requiredHours;
	string prefix{ "My work involves" };
};

class Programmer : public Job
{

public:

	Programmer(const string& name_, const string& description_, uint32_t requiredHours_) : Job(name_, description_, requiredHours_)
	{

	}

	virtual void DoWork() override
	{

		puts(format("{} {}.", GetPrefix(), GetDescription()).c_str());//faster than cout
	}
};

class Pilot : public Job
{

public:

	Pilot(const string& name_, const string& description_, uint32_t requiredHours_) : Job(name_, description_, requiredHours_)
	{

	}

	virtual void DoWork() override
	{

		puts(format("{} {}.", GetPrefix(), GetDescription()).c_str()); //faster than cout
	}
};

//Can handle multiple types and multiple arguments
//The assumption is that last parameter two parameters would be the start and end of the range
//Any parameters before the last two would be considered for bound checkig. 
//If any one of them fails return false otherwise true.
template<typename... Args>
bool IsInBounds(Args... args)
{
	constexpr auto numargs{ sizeof... (args) };
	if (numargs < 3)
		return false;

	vector<uint32_t> vargs{ static_cast<uint32_t>(args)... }; //compiler warning C4838 - int to uint32_t is okay no data loss

	uint32_t end = vargs.back();
	vargs.pop_back();

	uint32_t start = vargs.back();
	vargs.pop_back();

	return all_of(vargs.begin(), vargs.end(), [=](uint32_t elem) {return (elem >= start && elem <= end); });
}

//uint32_t ContainsTheString(std::function<bool(const string&)> fn, vector<string> theStrings) {
//uint32_t ContainsTheString(auto fn, vector<string> theStrings) {
//Can support any type which supports operator==()
uint32_t ContainsTheString(auto fn, auto theStrings)
{

	uint32_t count = 0;
	for (auto str : theStrings) {
		if (fn(str))
			++count;
	}

	return count;
}


void print(uint32_t httpResponseCode, uint32_t rangeStart, uint32_t rangeEnd, bool inRange)
{
	puts(format("Start {} End {} Value {} : {}.", rangeStart, rangeEnd, httpResponseCode, (inRange) ? "In Range" : "Out Of Range").c_str());
}

void printValuesRangeResult() 
{
	cout << endl;
}

template <typename T> 
void printValuesRangeResult(const T& t) 
{
	cout << t << endl;
}

template <typename First, typename... Rest>
void printValuesRangeResult(const First& first, const Rest&... rest)
{
	cout << first << " ";
	printValuesRangeResult(rest...);
}

//Unit Test Functions
void unit_test_ClassHierarchy(void);
void unit_test_IsInBounds(void);
void unit_test_IsInBoundsMultipleValues(void);
void unit_test_ContainsTheString(void);
void unit_test_ContainsTheString_Additional(void);

int main()
{
	cout << endl << "*********Class Hierarchy Testing*********" << endl;

	//Dynamically allocate each type of job
	vector<shared_ptr<Job>> vJobs
	{
		make_shared<Programmer>("Programmer", "Computer Programmer", 4),
		make_shared<Pilot>("Pilot", "Aviation Pilot", 40)
	};

	for (auto job : vJobs)
	{
		job->GetName();
		job->GetDescription();
		job->GetRequiredHours();
		job->DoWork();
	}

	//unit test for ClassHierarchy
	unit_test_ClassHierarchy();


	cout << endl << "*********Range Testing*********" << endl;

	//Basic test as defined in the instruction

	//in the range - before the begining
	uint32_t rangeStart = 500;
	uint32_t rangeEnd = 599;

	//in the range - in the middle
	uint32_t httpResponseCode = 550;
	bool inBound = IsInBounds(httpResponseCode, rangeStart, rangeEnd);
	print(httpResponseCode, rangeStart, rangeEnd, inBound);

	//unit test for IsInBounds
	unit_test_IsInBounds();

	//unit test for multiple values with variadic template
	unit_test_IsInBoundsMultipleValues();


	cout << endl << "*********String match test*********" << endl;

	string search = "test";
	auto theStrings = vector<string>{ "one", "two", "test","test" };
	auto count = ContainsTheString([search](const std::string& tested) { return tested == search; }, theStrings);
	cout << "The string " << search << " found " << count << " times." << endl;

	//unit test for ContainsTheString
	unit_test_ContainsTheString();

	//Additional featured ContainsTheString tests for other types 
	unit_test_ContainsTheString_Additional();

	return 0;
}


void unit_test_ClassHierarchy(void)
{
	cout << endl << "unit_test_ClassHierarchy" << endl;

	//Dynamically allocate each type of job
	vector<shared_ptr<Job>> vJobs
	{
		make_shared<Programmer>("Programmer", "Computer Programmer1", 1),
		make_shared<Programmer>("Programmer", "Computer Programmer2", 2),
		make_shared<Programmer>("Programmer", "Computer Programmer3", 3),
		make_shared<Pilot>("Pilot", "Aviation Pilot1", 10),
		make_shared<Pilot>("Pilot", "Aviation Pilot2", 20),
		make_shared<Programmer>("Programmer", "Computer Programmer4", 4),
		make_shared<Programmer>("Programmer", "Computer Programmer5", 5),
		make_shared<Programmer>("Programmer", "Computer Programmer6", 6),
		make_shared<Pilot>("Pilot", "Aviation Pilot3", 30),
		make_shared<Pilot>("Pilot", "Aviation Pilot4", 40),
		make_shared<Pilot>("Pilot", "Aviation Pilot5", 50)
	};

	for (auto job : vJobs)
	{

		job->GetName();
		job->GetDescription();
		job->GetRequiredHours();
		job->DoWork();
	}

}

void unit_test_IsInBounds(void)
{
	cout << endl << "unit_test_IsInBounds" << endl;
	//Basic test as defined in the instruction

	//in the range - before the begining
	uint32_t rangeStart = 500;
	uint32_t rangeEnd = 599;

	uint32_t httpResponseCode = 499;
	bool inBound = IsInBounds(httpResponseCode, rangeStart, rangeEnd);
	print(httpResponseCode, rangeStart, rangeEnd, inBound);
	assert(inBound == false);

	//in the range - at the begining
	httpResponseCode = 500;
	inBound = IsInBounds(httpResponseCode, rangeStart, rangeEnd);
	print(httpResponseCode, rangeStart, rangeEnd, inBound);
	assert(inBound == true);

	//in the range - in the middle
	httpResponseCode = 550;
	inBound = IsInBounds(httpResponseCode, rangeStart, rangeEnd);
	print(httpResponseCode, rangeStart, rangeEnd, inBound);
	assert(inBound == true);

	//in the range - at the end
	httpResponseCode = 599;
	inBound = IsInBounds(httpResponseCode, rangeStart, rangeEnd);
	print(httpResponseCode, rangeStart, rangeEnd, inBound);
	assert(inBound == true);

	//in the range - after the end
	httpResponseCode = 600;
	inBound = IsInBounds(httpResponseCode, rangeStart, rangeEnd);
	print(httpResponseCode, rangeStart, rangeEnd, inBound);
	assert(inBound == false);


}

void unit_test_IsInBoundsMultipleValues(void)
{
	cout << endl << "unit_test_IsInBoundsMultipleValues" << endl;

	//in the range - before the begining
	uint32_t rangeStart = 500;
	uint32_t rangeEnd = 599;

	bool inBound = IsInBounds(500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, rangeStart, rangeEnd);
	printValuesRangeResult("All of the vales ", 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, " are in the range ", rangeStart, "-", rangeEnd);
	assert(inBound == true);

	inBound = IsInBounds(499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, rangeStart, rangeEnd);
	printValuesRangeResult("Some of the values ", 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, " are not in the range ", rangeStart, "-", rangeEnd);
	assert(inBound == false);
}

void unit_test_ContainsTheString(void)
{
	cout << endl << "unit_test_ContainsTheString" << endl;
	auto theStrings = vector<string>
	{
		"one",
		"two",
		"three", "three", "three",
		"four",
		"five", "five", "five", "five", "five",
		"six",
		"seven", 
		"eight", "eight", "eight", "eight", "eight", "eight", "eight", "eight",
		"nine",
		"ten", "ten", "ten", "ten", "ten", "ten", "ten", "ten", "ten", "ten",
	};

	string search = "test";
	auto count = ContainsTheString([search](const std::string& tested) { return tested == search; }, theStrings);
	cout << "The string " << search << " found " << count << " times." << endl;
	assert(count == 0);

	search = "one";
	count = ContainsTheString([search](const std::string& tested) { return tested == search; }, theStrings);
	cout << "The string " << search << " found " << count << " times." << endl;
	assert(count == 1);

	search = "three";
	count = ContainsTheString([search](const std::string& tested) { return tested == search; }, theStrings);
	cout << "The string " << search << " found " << count << " times." << endl;
	assert(count == 3);

	search = "five";
	count = ContainsTheString([search](const std::string& tested) { return tested == search; }, theStrings);
	cout << "The string " << search << " found " << count << " times." << endl;
	assert(count == 5);

	search = "eight";
	count = ContainsTheString([search](const std::string& tested) { return tested == search; }, theStrings);
	cout << "The string " << search << " found " << count << " times." << endl;
	assert(count == 8);

	search = "ten";
	count = ContainsTheString([search](const std::string& tested) { return tested == search; }, theStrings);
	cout << "The string " << search << " found " << count << " times." << endl;
	assert(count == 10);
}


void unit_test_ContainsTheString_Additional(void)
{
	cout << endl << "unit_test_ContainsTheString_Additional" << endl;

	vector<int> theInts{ 1, 2, 3, 6, 6, 6, 6, 6, 6 };
	int search = 6;
	auto count = ContainsTheString([search](const int& tested) { return tested == search; }, theInts);
	cout << "The int " << search << " found in " << count << " times." << endl;
	assert(count == 6);
}