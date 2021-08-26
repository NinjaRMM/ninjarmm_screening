// steptwo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>


class Job {
public:
	virtual std::string getName() const =0;
	virtual std::string getDescription() const = 0;
	virtual double getHours() const = 0;
	virtual void doWork() =0;
};

class Pilot : public Job
{
public:
	std::string _Name;
	std::string _Description;
	double _Hours;

    Pilot(std::string Name, std::string Description, double hours)
	{
		this->_Name=Name;
		this->_Description=Description;
		this->_Hours=hours;
	}
   
    std::string getName() const override
    {
        return this->_Name;
    }
    std::string getDescription() const override
    {
        return this->_Description;
    }
    double getHours() const override
    {
        return this->_Hours;
    }
    void doWork() override
	{
		std::cout << "My work involves " + getDescription();
	}
};

class Programmer : public Job
{
public:
	std::string _Name;
	std::string _Description;
	double _Hours;
    Programmer(std::string Name, std::string Description, double hours)
	{
		this->_Name=Name;
		this->_Description=Description;
		this->_Hours=hours;
	}
   
    std::string getName() const override
    {
        return this->_Name;
    }
    std::string getDescription() const override
    {
        return this->_Description;
    }
    double getHours() const override
    {
        return this->_Hours;
    }
    void doWork() override
	{
		std::cout << "My work involves " + getDescription() << "\n";
	}
};
/*
h. Create a template function that will test if the provided values are
 within a range.
	Call the function and print out the inputs and result. The following 
	should execute
	assuming httpResonse is a uint32_t value:
		i. IsInBounds<uint32_t>(httpResonseCode, 500, 599)
*/

template<typename X>
bool IsInBounds( X valTest, const X valLow, const X valHigh) {
	if( valTest >= valLow && valTest <= valHigh) {
		return true;
	}
	return false;
}

/*	i. Create a function that will take a vector of strings 
	and a test function, It should return 
	how many items in the collection matched the test. 
	Then print out the result.
*/

// void do_something(T val, std::function<void(T)> p) 
//	 auto count = ContainsTheString([](const std::string& tested)
//		{ return tested == "test" }, theStrings);

auto theStrings = std::vector<std::string> { "one", "two",  "test"};
auto theInts = std::vector<int> { 1,4,3,5,9,2,0,4 };

int ContainsTheString( const std::function<int(std::string)> &func, 
		std::vector<std::string>& values)
{
	int num=0;
	for( std::string value : values)
	{
		num += func( value);
	}
	std::cout << num << std::endl;
	return num;
}

void ForEach( std::vector<int>& values, const std::function<int(int)> &func)
{
	int num=0;
	for( int value : values)
	{
		num += func( value);
	}
	std::cout << num << std::endl;
}
  
int main()
{
	uint32_t val1 = 0, val2=501, val3=700;
	std::cout << " 0 " <<  IsInBounds<uint32_t>( val1, 500, 599) << "\n";
	std::cout << "501 " << IsInBounds<uint32_t>( val2, 500, 599) << "\n";
	std::cout << "700 " << IsInBounds<uint32_t>( val3, 500, 599) << "\n";

	auto Lambda = [](int value) { return value > 3;};

	auto Lambda2 = [](const std::string& tested) { return tested == "test"; };

	ForEach( theInts, Lambda);

	std::cout<< ContainsTheString(  Lambda2, theStrings) << std::endl;

	std::vector<Job*> aVector;
	try {


		Programmer* aJob1 = new Programmer("Phil", "the programmer", 500.0);

		Pilot* aJob2 = new Pilot("Pat", "the Pilot", 600.0);

		aVector.push_back(aJob1);
		aVector.push_back(aJob2);

		for ( auto &j : aVector)
		{
			std::cout << j->getName() << "\n" << j->getDescription() <<
				"\n" << j->getHours() << "\n";
			j->doWork();
		}


	}
	catch (std::exception& e) {
		std::cout << e.what() << "\n";
	}
}