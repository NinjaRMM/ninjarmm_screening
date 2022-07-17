#include <iostream>
#include <string>
#include <list>
#include <vector>

// Job interface
class Job
{
public:
	virtual std::string getJob() = 0;
	virtual std::string getJobDescription() = 0;
	virtual int getWorkHours() = 0;
	virtual void doWork() = 0;
};

// Programmer class
class Programmer : public Job
{
private:
	std::string job = "Programmer";
	std::string jobDescription = "Developing code";
	int workHours = 8;
public:
	std::string getJob() override
	{
		return job;
	}

	std::string getJobDescription() override
	{
		return jobDescription;
	}

	void doWork() override
	{
		std::cout << "My work involves " + jobDescription << std::endl;
	}

	int getWorkHours() override
	{
		return workHours;
	}
};

// Pilot class
class Pilot : public Job
{
private:
	std::string job = "Pilot";
	std::string jobDescription = "Flying airplanes";
	int workHours = 10;
public:
	std::string getJob() override
	{
		return job;
	}

	std::string getJobDescription() override
	{
		return jobDescription;
	}

	void doWork() override
	{
		std::cout << "My work involves " + jobDescription << std::endl;
	}

	int getWorkHours() override
	{
		return workHours;
	}
};

// Template function IsInBounds
template <typename T>
void IsInBounds(const uint32_t& value, const int& lowerLimit, const int& upperLimit)
{
	std::cout << "Testing if " << value << " is withing the range of " << lowerLimit << " and " << upperLimit << std::endl;

	if (value >= lowerLimit && value <= upperLimit)
	{
		std::cout << value << " is withing the range!" << std::endl;
	}
	else
	{
		std::cout << value << " is NOT withing the range!" << std::endl;
	}
}

// Function that checks how many values from the input vector match with the test vector
int checkMatch(std::vector<std::string> vectorOfStrings)
{
	// Initializing the test vector
	std::vector<std::string> test = {"one", "three"};
	int count = 0;

	// Comparing every element of the input vector with the test vector
	for (int i = 0; i < vectorOfStrings.size(); i++)
	{
		for (int j = 0; j < test.size(); j++)
		{
			if (vectorOfStrings.at(i) == test.at(j))
			{
				count++;
			}
		}
	}

	return count;
}

// Function that invokes the checkMatch function 
int invoke(std::vector<std::string> vectorOfStrings, int (*func)(std::vector<std::string>))
{
	return func(vectorOfStrings);
}

int main()
{
	// Creating new objects of the class programmer and pilot
	Programmer* programmer = new Programmer();
	Pilot* pilot = new Pilot();

	// Creating the collections to store created instances 
	std::list<Programmer> programmerList;
	programmerList.push_back(*programmer);

	std::list<Pilot> pilotList;
	pilotList.push_back(*pilot);

	// Creating an iterator for the programmer list
	std::list<Programmer>::iterator it;

	// Iterating through the programmer collection
	for (it = programmerList.begin(); it != programmerList.end(); it++)
	{
		// Accessing the object through iterator
		std::cout << "Job: " << it->getJob() << std::endl;
		std::cout << "Description: " << it->getJobDescription() << std::endl;
		it->doWork();
		std::cout << "Work hours: " << it->getWorkHours() << std::endl;
		std::cout << std::endl;
	}

	// Creating an iterator for the programmer list
	std::list<Pilot>::iterator it2;

	// Iterating through the pilot collection
	for (it2 = pilotList.begin(); it2 != pilotList.end(); it2++)
	{
		// Accessing the object through iterator
		std::cout << "Job: " << it2->getJob() << std::endl;
		std::cout << "Description :" << it2->getJobDescription() << std::endl;
		it2->doWork();
		std::cout << "Work hours: " << it2->getWorkHours() << std::endl;
	}

	std::cout << std::endl;

	// Testing the IsInBounds function
	uint32_t httpResonseCode = 510;
	IsInBounds<uint32_t>(httpResonseCode, 500, 599);

	std::cout << std::endl;

	std::vector<std::string> vecStr = {"one", "two", "three"};

	std::cout << "Number of matched elements: " << invoke(vecStr, &checkMatch) << std::endl;

	//deleting the objects
	delete programmer;
	delete pilot;

	return 0;
}