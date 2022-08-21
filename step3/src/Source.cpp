/*
 * 
 * NINJARMM Step 3 
 * 
 * source code to be run on https://cpp.sh/  with C++20 selected
 *  
 * This file contains the 'main' function. Program execution begins and ends there.
 */

/*
 * Requirements:

1. Write a small program in C++ that will contain the following:
	a. Create a class Job
	b. Create a subclass Programmer
	c. Create a subclass Pilot
	d. The job class should provide an interface that can do the following
		i. Get the name of the job
		ii. Get the description of the job. 
		iii. Get the number of hours required by the job
		iv. A DoWork function that prints out"My work involves " + the description.  
	e. Dynamically allocate each type of job.
	f. Store each instance in a collection.
	g. Iterate the collection and call each of the available interface functions.
	h. Create a template function that will test if the provided values are within a range. Call the function and print out the inputs and result. The following should execute assuming httpResonse is a uint32_t value:
		i. IsInBounds<uint32_t>(httpResonseCode, 500, 599)
	i. Create a function that will take a vector of strings and a test function, It should return how many items in the collection matched the test. Then print out the result.
		i. auto theStrings = vector<std::string> { one, two,  test};
		ii. auto count = ContainsTheString([](const std::string& tested) { return tested == test }, theStrings);
	j. Write any other code you feel would show off your understanding of C/C++ and programming in general. Bonus: Variadic Template /w usage. 
	k. All The source can be in 1 file, it should compile and there should be no runtime errors, and no memory leaks. Use built in types freely to help you manage resources. 

 */
 
#include <iostream>
#include <map>
#include <vector>
#include <functional>

using namespace std;

//--- 1.a / 1.d START
class Job	//base class
{
	string name;
	string description;
	
	unsigned int hoursRequired;
	
	public:
		string getName()
		{
			return name;
		}
		void setName( string n )
		{
			name = n;
		}
		
		string getDescription()
		{
			return description;
		}
		void setDescription( string d )
		{
			description = d;
		}
		
		unsigned int getHoursRequired()
		{
			return hoursRequired;
		}
		void setHoursRequired( unsigned int h )
		{
			hoursRequired = h;
		}
		
		Job()
		{
		    name = "Job";
		    description = "Basic Job";
		    hoursRequired = 12;
		}
		
		virtual ~Job()
		{
		}
		
		void DoWork()
		{
			cout << "My work involves " << this -> getDescription() << endl;
		}
};
//--- 1.a 1.d END

//--- 1.b START
class Programmer : public Job	//derived class
{
	public:
        Programmer()
        {
		    setName("Programmer");
		    setDescription("Programming computers");
		    setHoursRequired(256);
        }
		virtual ~Programmer()
		{
		}
};
//--- 1.b END

//--- 1.c START
class Pilot : public Job	//derived class
{
	public:
        Pilot()
        {
		    setName("Pilot");
		    setDescription("Flying planes");
		    setHoursRequired(1200);
        }
		virtual ~Pilot()
		{
		}
};
//--- 1.c END

//--- 1.h START
template <typename T>
void IsInBounds(T value, T lowerValue, T upperValue)
{
    std::cout << "\n\t" << "Value: " << value;
    std::cout << "\n\t" << "lowerValue: " << lowerValue;
    std::cout << "\n\t" << "upperValue: " << upperValue;

    if ((value >= lowerValue) && (value <= upperValue))
    {
        std::cout << "\n\t" << "is IN Bounds";
    }
    else
    {
        std::cout << "\n\t" << "is NOT In Bounds";
    }
    
    std::cout << "\n" ;
    
    return;
}
//--- 1.h END

//--- 1.i START
unsigned int ContainsTheString(function<bool(const std::string&)> testFunction, std::vector<std::string> &vector)
{
    unsigned int counter = 0;
 
    for (std::string strValue : vector) 
    {
        if (testFunction(strValue))
            counter += 1;
    }
    return counter;
}
//--- 1.i END

//--- 1.j START Variadic Template
template<typename T>
T adder(T v) {
  return v;
}

template<typename T, typename... Args>
T adder(T first, Args... args) {
  return first + adder(args...);
}
//--- 1.j END Variadic Template

/*
 Create a function that will take a vector of strings and a test function, It should return how many items in the collection matched the test. Then print out the result.
		i. auto theStrings = vector<std::string> { "one", "two",  "test"};
		ii. auto count = ContainsTheString([](const std::string& tested) { return tested == "test" }, theStrings);
*/		
		
int main()
{
	
//--- 1.e START
    Programmer *programmer1 = new Programmer();
    Pilot *pilot1 = new Pilot();
//--- 1.e END
    
//--- 1.f START
    std::map<std::string, Job *> JobMap;
    
    JobMap[programmer1->getName()] = programmer1;
    JobMap[pilot1->getName()] = pilot1;
//--- 1.f END
    
//--- 1.g START
	std::cout << "\n\n 1.g OUTPUT \n";
	
    for (std::map<std::string, Job *>::iterator itr = JobMap.begin(); itr != JobMap.end(); ++itr) 
    {
        std::cout << "\n\t"
        << "Job Name: " << itr->second->getName() << "\n\t"  
        << "Job Description: "<< itr->second->getDescription() << "\n\t" 
        << "Job Number of Hours Required: " << itr->second->getHoursRequired()
             << "\n\t";    

        itr->second->DoWork();
    }

    delete programmer1;
    delete pilot1;
//--- 1.g END
    
//--- 1.h.i START
	std::cout << "\n\n 1.h.i OUTPUT \n";
	
    uint32_t httpResonseCode = 200;
    IsInBounds<uint32_t>(httpResonseCode, 500, 599);

    httpResonseCode = 500;
    IsInBounds<uint32_t>(httpResonseCode, 500, 599);
    
    std::string word = "here";
    IsInBounds<std::string>(word, "a", "z");

    word = "here";
    IsInBounds<std::string>(word, "i", "z");
//--- 1.h.i END
    
//--- 1.i.i START
    auto theStrings = std::vector<std::string>{ "one", "two", "test"};
//--- 1.i.i END

//--- 1.i.ii START
	std::cout << "\n\n 1.i.ii OUTPUT \n";
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    std::cout << "\n\t"
        << "Matches comparing to \"test\" -> " << count << "\n";
        
//--- 1.i.ii END

//--- 1.j Variadic Template START
	std::cout << "\n\n 1.j Variadic Template OUTPUT \n";
    long sum = adder(1, 2, 3, 8, 7);

    std::cout << "\n\t"
        << "adder with 5 parameters -> " << sum << "\n";
        
    sum = adder(1, 2, 3, 8, 7, 6, 8, 2, 4, 1);

    std::cout << "\n\t"
        << "adder with 10 parameters -> " << sum << "\n";
//--- 1.j Variadic Template END
        
    return 0;
}
