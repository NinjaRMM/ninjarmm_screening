//
// I run this using cpp.sh C++14 standards
//

#include <iostream>
#include <tuple>
#include <vector>
#include <memory>
#include <functional>

//
// Job base class provides the following interfaces:GetName,GetTime,GetDescription,DoWork,DisplayJob
// Note: all the interfaces have default implementations to allow thin specific Job classes (see Programmer)
//
template <typename...DataPartsArgs>
class Job
{
private:
    //
    // NOTE:in case there is a need to extend the interfaces with default behaviour add an index and corrosponding GET function see examples below
    //
    enum Index{Name, Description, Time};
    const std::tuple<DataPartsArgs...> m_dataPartsArgs;
    const std::string m_doWork = "My work involves " + std::get<Description>(m_dataPartsArgs);
protected:
    Job(const std::tuple<DataPartsArgs...>& dataPartsArgs):m_dataPartsArgs(dataPartsArgs){/*std::cout<<"Job:"<<*this;*/}
public:
    const std::string& GetName() const  {return std::get<Name>(m_dataPartsArgs);}
    const std::string& GetDescription() const  {return std::get<Description>(m_dataPartsArgs);}
    const uint32_t& GetTime() const  {return std::get<Time>(m_dataPartsArgs);}
    const std::string& DoWork(void) const {return m_doWork;}
    friend std::ostream& operator<<(std::ostream& out, const Job& job){return job.DisplayJob(out);}
    virtual std::ostream& DisplayJob(std::ostream& out) const {return out<<"{name:"<<GetName()<<",description:"<<GetDescription()<<",timeInHours:"<<GetTime()<<"}\n";}
    virtual ~Job(){/*std::cout<<"~Job"<<*this;*/}
};

//
// Specific jobs interface
// NOTE: in case Job interface is extended modify this template signature 
//
using JobInterfaceTemplate= Job<const std::string, const std::string, const uint32_t>;

//
// Programmer let's job manage basic data but overrides display job interface
//
class Programmer:public JobInterfaceTemplate
{
public:
    Programmer(uint32_t time,std::string && description="making software applications"):Job(std::make_tuple("programmer",std::move(description), time)){description = nullptr;/*std::cout<<*this;*/}
    std::ostream& DisplayJob(std::ostream& out) const override{out<<"SpecificJob:"; return Job::DisplayJob(out);}
    ~Programmer(){/*std::cout<<"~"<<*this;*/}
};

//
// Pilot, let's job manage basic data but overrides display job interface
//
class Pilot:public JobInterfaceTemplate
{
public:
    Pilot(uint32_t time, std::string && description="flying planes"):Job(std::make_tuple("pilot",std::move(description), time)){ description = nullptr; /*std::cout<<*this;*/}
    std::ostream& DisplayJob(std::ostream& out) const override{out<<"SpecificJob:"; return Job::DisplayJob(out);}
    ~Pilot(){/*std::cout<<"~"<<*this;*/}
};

//
// Check if value is in range [floor,ceiling]
//
template<typename Type>
bool IsInBounds(Type floor, Type ceiling, Type value)
{
    return (value >= floor && value <= ceiling);
}

//
// Stop condition for recursive varidac function
//
bool DoInBoundTestCase()
{
    return true;
}

//
// verify atucl == expected, unpack vardiac args via recursion
//
template<typename T, typename...Args>
bool DoInBoundTestCase(T floor, T ceiling, T value, bool expectedResult, Args... args)
{
    bool actualResult = IsInBounds(value,floor,ceiling);
    if(expectedResult == actualResult)
    {
        std::cout<<value<<":["<<floor<<","<<ceiling<<"]"<<(actualResult ? "=>InRange":"=>OutOfRange") <<"(Expected)"<<std::endl;
        return DoInBoundTestCase(args...);
    }
    else
    {
        std::cout<<value<<":["<<floor<<","<<ceiling<<"]"<<(actualResult ? "=>InRange":"=>OutOfRange") <<"(NotExpected)"<<std::endl;
        return false;   
    }
}

//
// Unit test for in bound template
//
void DoInBoundUnitTest()
{
    
    std::cout<<"->InBoundUnitTest"<<std::endl;
    
    //------------------------------Floor | Ceiling | Value | InBound -----------------
	if(!DoInBoundTestCase<uint32_t>( 500,      599,     600,   false,
	                                 500,      599,     599,   true,
	                                 500,      599,     499,   false))
	{
	    std::cout<<"uint32 inbound unit test failed!!" << std::endl;
	}
	//-------------------------Floor | Ceiling | Value | InBound ----------------------
	if(!DoInBoundTestCase<int>( -100,     100,    -101,   false,
	                            -100,     100,       0,   true,
	                            -100,     100,     101,   false))
	{
	    std::cout<<"int inbound unit test failed!!" << std::endl;
	}
	
	//
	// TBD: Add test cases
	//
	
	std::cout<<"<-InBoundUnitTest"<<std::endl;
}

//
// Count string in vectors using algorithim STL
//
int ContainsTheString(std::function<bool(const std::string& theString)> testFunc, const std::vector<std::string> & theStrings)
{
    return std::count_if(theStrings.cbegin(), theStrings.cend(), testFunc);
}

//
// Stop condition for vardiac recursion
//
bool DoStringCountTestCase(std::vector<std::string> & theStrings)
{
    return true;   
}

//
// verify actual == expected and unpack vardiac args via recursion
//
template<typename...Args>
bool DoStringCountTestCase(std::vector<std::string> & theStrings, std::string theString, int expectedCount, Args...args)
{
    int actualCount = ContainsTheString([&theString](const std::string& tested){ return tested == theString;},theStrings);
    if(expectedCount == actualCount)
    {
        std::cout <<"\""<<theString <<"\" count=" << actualCount <<"(Expected)"<<std::endl;
        return DoStringCountTestCase(theStrings, args...);
    }
    else
    {
        std::cout <<"\""<<theString <<"\" count=" << actualCount <<"(NotExpected)"<<std::endl;
        return false;   
    }
}

//
// Unit test for string count
//
bool DoStringCountUnitTest()
{
    std::cout<<"->StringCountUnitTest"<<std::endl;
    
    auto theStrings = std::vector<std::string> {"one", "two", "three","four", "two", "three", "three", "four", "four", "four"};
	if (!DoStringCountTestCase<std::string>(theStrings, // string, expectedCount
	                                                       "zero",     0,  
	   	                                               "one",      1,
	   	                                               "two",      2,
	                                                       "three",    3,
	   	                                               "four",     4))
	{
	    std::cout<<"string count unit test failed!!" << std::endl;
	}	
	
	//
	// TBD: Add test cases
	//
	
	std::cout<<"<-StringCountUnitTest"<<std::endl;
	return true;
}

//
// Main entry point
//
int main()
{
    std::vector<std::unique_ptr<JobInterfaceTemplate>> jobs;
    jobs.push_back(std::make_unique<Programmer>(10));
    jobs.push_back(std::make_unique<Pilot>(20));
    jobs.push_back(std::make_unique<Programmer>(20,"make embedded applications"));
    jobs.push_back(std::make_unique<Pilot>(20,"fly single engine planes"));
    jobs.push_back(std::make_unique<Programmer>(30,"make server applications"));
    jobs.push_back(std::make_unique<Pilot>(40,"fly double engine planes"));
    
    //
    // Invoke virtual function 
    //
    for (const auto & job : jobs)
    {
   	std::cout<<*job;
    }
	
    //
    // Invoke non virtual functions 
    //
    for (const auto & job : jobs)
    {
   	std::cout<<"{job:"<<job->GetName()<<",description:"<<job->GetDescription()<<",time:"<<job->GetTime()<<",DoWork:"<<job->DoWork()<<"}\n";	
    }
	
    // 
    // Do unit testing
    //
    DoInBoundUnitTest();
    DoStringCountUnitTest();
}
