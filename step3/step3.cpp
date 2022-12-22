
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <queue>
#include <condition_variable>
#include <thread>
#include <chrono>

//IMPORTANT: this module uses thread lib, use -pthread to link it: g++ step3.cpp  -pthread

//Since the exercise must be standalone, I have decided not use a specific Testing framework as Google Test. 
//This is a little adoc macro defined to emulate the classic macros offered by the testing frameworks
#define ASSERT_EQUAL(expected, value)   \
        do {                                        \
            if (expected != value)        \
            { \
                std::cout << "FAIL: Expected " << expected << " value " << value << std::endl; \
                exit(1) ;  \
            }                             \
            else\
            {\
                std::cout << "OK: Expected " << expected << " value " << value << std::endl; \
            }\
        } while(0)

//1. Write a small program in C++ that will contain the following:
//	a. Create a class Job
//	b. Create a subclass Programmer
//	c. Create a subclass Pilot
//	d. The job class should provide an interface that can do the following
//		i. Get the name of the job
//		ii. Get the description of the job. 
//		iii. Get the number of hours required by the job
//		iv. A DoWork function that prints out “My work involves ” + the description.  
//	e. Dynamically allocate each type of job.
//	f. Store each instance in a collection.
//	g. Iterate the collection and call each of the available interface functions.

class Job
{
    public:

    virtual ~Job(){};

    virtual std::string GetName() = 0; 

    virtual std::string GetDescription() = 0;

    virtual int GetHoursRequired() = 0;

    void DoWork(); 

};

void Job::DoWork()
{
    std::cout << (std::string("My work involves ")+ GetDescription()) << std::endl;
}

class Programmer : public Job
{
    public:

    virtual ~Programmer(){};

    virtual std::string GetName(); 

    virtual std::string GetDescription();

    virtual int GetHoursRequired();

};

std::string Programmer::GetName()
{
    return "Programmer";
}

std::string Programmer::GetDescription()
{
    return "design, develop and test software";
}

int Programmer::GetHoursRequired()
{
    return 8;
}

class Pilot : public Job
{
        public:

    virtual ~Pilot(){};

    virtual std::string GetName(); 

    virtual std::string GetDescription();

    virtual int GetHoursRequired();


};

std::string Pilot::GetName()
{
    return "Pilot";
}

std::string Pilot::GetDescription()
{
    return "flight a plane";
}

int Pilot::GetHoursRequired()
{
    return 10;
}

//	h. Create a template function that will test if the provided values are within a range. Call the function and print out the inputs and result. 
//  The following should execute assuming httpResonse is a uint32_t value:
//		i. IsInBounds<uint32_t>(httpResonseCode, 500, 599)

template <typename T>
bool IsInBounds(T code, T min, T max)
{
    return (code >= min) && (code <= max);
}

//	i. Create a function that will take a vector of strings and a test function, It should return how many items in the collection matched the test. Then print out the result.
//		i. auto theStrings = vector<std::string> { “one”, “two”,  “test”};
//		ii. auto count = ContainsTheString([](const std::string& tested) { return tested == “test” }, theStrings);

template<typename F>
uint32_t ContainsTheString(F && lambda, const std::vector<std::string> & texts)
{
    uint32_t count=0;

    for (const auto t : texts)
    {
        if (lambda(t))
        {
            count++;
        }
    }
    return count;
}

//	j. Write any other code you feel would show off your understanding of C/C++ and programming in general. Bonus: Variadic Template /w usage. 
// This is a little module that recently implemented to add and consume events from a FIFO queue
class ConsumerProducer
{
    public:

    ConsumerProducer():eventThread(&ConsumerProducer::SendEventsTask, this)
    {
    }

    void JoinEventThread(){eventThread.join();}

    void SendEventsTask()
        {
            while (1)
            {
                std::unique_lock<std::mutex> lck(eventHandle.mutex);

                eventHandle.conditionVariable.wait(lck, [this]{ return eventHandle.IsThereEvent();});
                std::cout << "SendEvents thread working" << std::endl;
                std::string event;
                if (eventHandle.PopNextEvent(event))
                {
                    SendEvent(event);
                }
            }
        }

    void AddTrackingEvent(const std::string &event)
    {
        // These operations are include in a lambda thread to execute them asynchronously and avoid lock the user of the function
        std::thread asynchExceution([this, event]
                                    {
                                        std::unique_lock<std::mutex> lck(eventHandle.mutex);
                                        eventHandle.PushEvent(event); 
                                    });
        asynchExceution.detach();
    }

    void SendEvent(const std::string &event)
    {
        std::cout << "sending :"  << event << std::endl;
    }

    protected: 
    /// Thread to send events
    std::thread eventThread;

    class
    {
    public:
        bool IsThereEvent()
        {
            return !queue.empty();
        }

        void PushEvent(const std::string &event)
        {
            queue.push(event);
            conditionVariable.notify_one();
        }
        bool PopNextEvent(std::string &event)
        {
            if (!queue.empty())
            {
                event = queue.front();
                queue.pop();
                return true;
            }
            return false;
        }

        std::condition_variable conditionVariable;
        std::mutex mutex;

    protected:
        std::queue<std::string> queue;
    } eventHandle;
};

//	k. Please add passing unit tests. 
//	l. All The source can be in 1 file, it should compile and there should be no runtime errors, and no memory leaks. Use built in types freely to help you manage resources. 


int main()
{

    //EXERCISE 1

    std::vector<std::shared_ptr<Job>> jobs;

    //Dynamically allocate each tuype of job
    std::shared_ptr<Job> job1 = std::make_shared<Programmer>();
    std::shared_ptr<Job> job2 = std::make_shared<Pilot>();

    //Store each instance in a collection
    jobs.push_back(job1);
    jobs.push_back(job2);

    //Iterate the collection and call each of the available interface functions
    for(const auto job : jobs)
    {
        std::cout << "Name " << job->GetName() << std::endl;
        std::cout << "Description " << job->GetDescription() << std::endl;
        std::cout << "Hours " << job->GetHoursRequired() << std::endl;
        job->DoWork();
    }

    //EXERCISE 2

    struct BoundTestDescriptor
    {
        uint32_t httpResponseCode;
        bool expectedResult;
    };
    BoundTestDescriptor boundCases[] ={{404,false},
                                       {501,true}};

    size_t boundCasesSize = sizeof(boundCases)/sizeof(BoundTestDescriptor);
    for (uint32_t i=0;i<boundCasesSize;i++)
     {
        ASSERT_EQUAL(IsInBounds<uint32_t>(boundCases[i].httpResponseCode, 500, 599),boundCases[i].expectedResult);
     }
    


    //EXERCISE 3
    struct ContainTestDescriptor
    {
        std::vector<std::string> theStrings;
        uint32_t expectedCound;
    };

    ContainTestDescriptor containCases[] = {{{"one", "two"},0},
                                            {{"one", "two", "test"},1},
                                            {{"test", "two", "test"},2},
                                            {{"test", "test", "test"},3}};

    size_t containsCasesSize = sizeof(containCases)/sizeof(ContainTestDescriptor);

    for (uint32_t i=0;i<containsCasesSize;i++)
    {
        auto count = ContainsTheString([](const std::string &tested){ return tested == "test"; }, containCases[i].theStrings);
        ASSERT_EQUAL(count,containCases[i].expectedCound);
    }


    //OPTIONAL, EXERCISE 4

    //Create the consumer producer system
    ConsumerProducer consumerProducerSystem;

    //Add the first vent
    consumerProducerSystem.AddTrackingEvent("Event1");
    //A little sleep to have time to check the system traces (in a real scenario it wont be required)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));    
    //A second event
    consumerProducerSystem.AddTrackingEvent("Event2");

    //Wait for the end of the task (never, it is an infinite task)
    consumerProducerSystem.JoinEventThread();
    
    return 0;
}


