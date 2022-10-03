#include <iostream>
#include <string>
#include <vector>

// 1. Write a small program in C++ that will contain the following:


// a. Create a class Job
class Job
{
protected:
	//Member Variables
	std::string newName;
	std::string newDescription;
	double newHours;

public:

	Job()
	{

	}

	// Destructor
	virtual ~Job()
	{

	}

	// d.The job class should provide an interface that can do the following

	// i. Get the name of the job
	virtual std::string getName() = 0;

	// ii. Get the description of the job
	virtual std::string getDescription() = 0;

	// iii. Get the number of hours required by the job 
	virtual double getHours() = 0;

	// iv. A DoWork function that prints out “My work involves ” + the description. 
	virtual void DoWork() = 0;
};

// b.Create a subclass Programmer
class Programmer : public Job
{
public:

	Programmer(std::string name, std::string description, double hours)
	{
		newName = name;
		newDescription = description;
		newHours = hours;
	}

	std::string getName()
	{
		return newName;
	}

	std::string getDescription()
	{
		return newDescription;
	}

	double getHours()
	{
		return newHours;
	}

	void DoWork()
	{
		std::cout << "My work involves " << getDescription() << std::endl;
	}
};

// Create a subclass Pilot
class Pilot : public Job
{
public:

	Pilot(std::string name, std::string description, double hours)
	{
		newName = name;
		newDescription = description;
		newHours = hours;
	}

	std::string getName()
	{
		return newName;
	}

	std::string getDescription()
	{
		return newDescription;
	}

	double getHours()
	{
		return newHours;
	}

	void DoWork()
	{
		std::cout << "My work involves " << getDescription() << std::endl;
	}
};

// h. Create a template function that will test if the provided values are within a range. 
//    Call the function and print out the inputs and result. 
//    The following should execute assuming httpResonse is a uint32_t value:
//		i.IsInBounds<uint32_t>(httpResonseCode, 500, 599)
template <typename T, typename...Ts>
uint32_t IsInBounds(const T& value, const T& low, const T& high...) {

	std::cout << "Max value: " << high << std::endl;
	std::cout << "Min value: " << low << std::endl;
	std::cout << "Value to be evaluated: " << value << std::endl;

	if (!(low < value) && !(high > value))
	{
		std::cout << "out of range" << std::endl;
		return -1;
	}

	std::cout << "in range" << std::endl;
	return 1;
}

bool ContainsTheString(const std::string& tested)
{
	return tested == "test";
}



int main()
{
	/*
		1. Write a small program in C++ that will contain the following :
			a.Create a class Job ok
			b.Create a subclass Programmer ok
			c.Create a subclass Pilot ok
			d.The job class should provide an interface that can do the following
				i.Get the name of the job ok
				ii.Get the description of the job.ok
				iii.Get the number of hours required by the job ok
				iv.A DoWork function that prints out “My work involves ” + the description.ok
			e.Dynamically allocate each type of job.ok
			f.Store each instance in a collection.ok
			g.Iterate the collection and call each of the available interface functions.ok
	*/

	std::vector<std::unique_ptr<Job>> jobList;

	std::string name = "SR C++ Engineer";
	std::string description = "SR C++ Engineer at NinjaOne: Work closely with clients to automate applications lifecycles.";
	double hours = 40;

	// e. Dynamically allocate each type of job.
	Programmer* programmer = new Programmer(name, description, hours);

	// f. Store each instance in a collection
	jobList.emplace_back(programmer);

	name = "American Navy Pilot";
	description = "Fly fighters";
	hours = 40;

	// e. Dynamically allocate each type of job.
	Pilot* pilot = new Pilot(name, description, hours);

	// f. Store each instance in a collection
	jobList.emplace_back(pilot);

	// g. Iterate the collection and call each of the available interface functions.
	for (unsigned int i = 0; i < jobList.size(); i++)
	{
		std::cout << "Name: " << jobList[i]->getName() << std::endl;
		std::cout << "Description: " << jobList[i]->getDescription() << std::endl;
		std::cout << "Hours: " << jobList[i]->getHours() << std::endl;
		jobList[i]->DoWork();
		std::cout << std::endl;
	}

	//----------------------------------------------------------------------------------------------------------------------------//

	/*
		h. Create a template function that will test if the provided values are within a range. Call the function and print out the inputs and result. The following should execute assuming httpResonse is a uint32_t value:
			i.IsInBounds<uint32_t>(httpResonseCode, 500, 599)
	*/


	//IsInBounds(10,  20, 30);
	IsInBounds(50, 10, 100, 200, 120, 150);
	//IsInBounds(50, 1, 2, 3, 10, 100);


	//----------------------------------------------------------------------------------------------------------------------------//

	/*
		i. Create a function that will take a vector of strings and a test function, It should return how many items in the collection matched the test. Then print out the result.
			i. auto theStrings = vector<std::string>{ “one”, “two”,  “test” };
			ii. auto count = ContainsTheString([](const std::string& tested) { return tested == “test” }, theStrings);
	*/

	int count = 0;
	auto theStrings = std::vector<std::string>{ "one", "two",  "test", "three", "four", "five", "test", "six", "tes", "seven", "t", "eight", "nine", "tests", "ten", "eleven", "twelve", "test" };

	for (unsigned int i = 0; i < theStrings.size(); i++)
	{
		if (ContainsTheString(theStrings[i]))
		{
			count++;
		}
	}


	std::cout << "Matched the test: " << count << std::endl;

	//----------------------------------------------------------------------------------------------------------------------------//


	return 0;
}



