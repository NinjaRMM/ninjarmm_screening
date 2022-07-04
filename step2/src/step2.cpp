// Daniel Vasconcelos Gomes
// 2022 June Testing NinjaOne

// Requirements
/**
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
	h. Create a template function that will test if the provided values are within a range. Call the function and print out the inputs and result. 
	The following should execute assuming httpResonse is a uint32_t value:
		i. IsInBounds<uint32_t>(httpResonseCode, 500, 599)
	i. Create a function that will take a vector of strings and a test function, It should return how many items in the collection matched the test. 
	Then print out the result.
		i. auto theStrings = vector<std::string> { “one”, “two”,  “test”};
		ii. auto count = ContainsTheString([](const std::string& tested) { return tested == “test” }, theStrings);
	j. Write any other code you feel would show off your understanding of C/C++ and programming in general. Bonus: Variadic Template /w usage. 
	k. All The source can be in 1 file, it should compile and there should be no runtime errors, and no memory leaks. Use built in types freely to help you manage resources. 
*/


#include <bits/stdc++.h>


// Base virtual class
class Job
{
public:
	Job(	const std::string& Name,
			const std::string& Description, 
			int Hours):Name{Name}, Description{Description}, Hours{Hours}
	{
	};	
	virtual ~Job()
	{
		std::cout << "Calling Job base class destructor" << std::endl;
	}	
	virtual void DoWork() const = 0;
	virtual std::string getName() const
	{
		return Name;
    };
	
	virtual std::string getDescription() const
	{
		return Description;
    };
	
	virtual int getHours() const
	{
		return Hours;
	};	
	
private:	
	std::string Name;
	std::string Description;
	int Hours;
};

// Derived programmer class
class Programmer: public Job
{
public:
	Programmer(	const std::string& Name,
				const std::string& Description, 
				int Hours):Job(Name,Description,Hours){};
	virtual ~Programmer()
	{
		std::cout << "Calling Programmer Destructor" << std::endl;
	}
	virtual void DoWork() const
	{
		std::cout << "My work involves " << getDescription() << std::endl;
	};

};

// Derived pilot class
class Pilot: public Job
{
public:
	Pilot(	const std::string& Name,
			const std::string& Description, 
			int Hours):Job(Name,Description,Hours){};
	virtual ~Pilot()
	{
		std::cout << "Calling Pilot Destructor" << std::endl;
	};
	virtual void DoWork() const
	{
		std::cout << "My work involves " << getDescription() << std::endl;
	};

};

// runs the DoWork method
void RunWork( const std::vector<Job*>& jobs )
{
	for( auto& j :jobs)
	{
		j->DoWork();
	}
}

// remove allocated memory
void clearContainer(std::vector<Job*>& jobs)
{
	for( auto& j :jobs)
	{
		if ( j != nullptr )
			delete j;
	}	
	jobs.clear();
}

template< typename V >
bool IsInBounds( const V& httpResonseCode, const V&MIN, const V& MAX)
{
	return ( httpResonseCode >= MIN && httpResonseCode <= MAX );
}

auto compare_it = 
[]( const std::string& s1, 
	const std::string& s2 )
{
	return  s1 == s2;
};
		
		
template<typename Lambda>
int testContainer(std::vector<std::string>& s, Lambda& f, std::string test)
{
	int cnt = 0;
	for( auto& it: s )
	{
		if ( f(test,it) == true )
			cnt++;
	}
	return cnt;
}


std::string foo()
{
	std::string something = "avalue";
	return something;
}

int main()
{
	
	// itens a to g
	std::vector<Job*> Jobs;	
	Jobs.emplace_back(new Programmer("Programmer","Programming",10));
	Jobs.emplace_back(new Pilot("Pilot","Flying",20 ));
	RunWork(Jobs);
	clearContainer(Jobs);
	
	// item h
	uint32_t httpResonseCode = 545;
	constexpr uint32_t MAX = 599;
	constexpr uint32_t MIN = 500;
	if ( IsInBounds<uint32_t>(httpResonseCode, MIN, MAX) == true )
	{
		std::cout << httpResonseCode << " is within bounds " << MIN << " and " << MAX << std::endl;
	}
	else
	{
		std::cout << httpResonseCode << " is not within bounds " << MIN << " and " << MAX << std::endl;
	}
	
	// item i
	auto theStrings = std::vector<std::string> { "one", "two",  "test", "test", "test"};
	std::string t = "test";
	int cnt = testContainer(theStrings,compare_it,t);
	std::cout << "Number of matches " << cnt << std::endl;
	

}


