#include <iostream>
#include <string>
#include <vector>

class Job 
{
public:
    Job(){}
    virtual ~Job(){};

    virtual const std::string getName() = 0;
    virtual const std::string getDescription() = 0;
    virtual const int getNumberOfHoursRequired() = 0;
    virtual const void DoWork() = 0;
    
protected:
    std::string name;
    std::string description;
    int requiredHours;
};

class Programmer : public Job
{
public:
    Programmer()
    {
        name = "Programmer";
        description = "developing software";
        requiredHours = 8;
    }

    virtual ~Programmer()
    {
        std::cout << "I am Programmer's destructor" << std::endl;
    }

    virtual const std::string getName()
    {
        return name;
    }
    
    virtual const std::string getDescription()
    {
        return description;
    }

    virtual const int getNumberOfHoursRequired()
    {
        return requiredHours;
    }

    virtual const void DoWork()
    {
        std::cout << "My work involves " << description << std::endl;
    }
};

class Pilot : public Job
{
public:
    Pilot()
    {
        name = "Pilot";
        description = "flying aircrafts";
        requiredHours = 20;
    }
    
    virtual ~Pilot()
    {
        std::cout << "I am Pilot's destructor" << std::endl;
    }

    virtual const std::string getName()
    {
        return name;
    }
    
    virtual const std::string getDescription()
    {
        return description;
    }

    virtual const int getNumberOfHoursRequired()
    {
        return requiredHours;
    }

    virtual const void DoWork()
    {
        std::cout << "My work involves " << description << std::endl;
    }
};

template<typename T>
void IsInBounds(T httpResonseCode, int start, int end)
{
    std::cout << "HttpResponseCode=[" << httpResonseCode <<"] range: start=[" << start << "] end=[" << end << "]" << std::endl;
    std::string isInBounds = httpResonseCode > start && httpResonseCode < end ? "TRUE" : "FALSE";
    std::cout << isInBounds << std::endl;
}

template<typename T>
int ContainsTheString(T lambda, std::vector<std::string> theStrings)
{
    int count = 0;
    std::vector<std::string>::iterator it;

    for( it = theStrings.begin(); it < theStrings.end(); ++it )
        if( lambda(*it) )
            count++;

    return count;
}

int main()
{
    Job *job1 = new Programmer();
    Job *job2 = new Pilot();

    std::vector<Job*> collection;
    collection.push_back(job1);
    collection.push_back(job2);

    std::vector<Job*>::iterator itr; 

    for( itr = collection.begin(); itr < collection.end(); ++itr)
    {
        Job *job = *itr;
        std::cout << "Job name: " << job->getName() << std::endl;
        std::cout << "Job desciption: " << job->getDescription() << std::endl;
        std::cout << "Job number of required hours: " << job->getNumberOfHoursRequired() << std::endl;
        job->DoWork();
    }

    delete job1;
    delete job2;
    
    // template function
    uint32_t httpResonseCode = 550;
    IsInBounds<uint32_t>(httpResonseCode, 500, 599);

    //
    auto theStrings = std::vector<std::string> { "one", "two",  "test"};
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    std::cout << "count: " << count << std::endl;
}