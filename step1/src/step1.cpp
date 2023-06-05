// Step1 C++ Program

#include <iostream>
#include <set>
#include <vector>
#include <queue>
#include <memory>
#include <functional>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <cstdlib>
using namespace std;

// Class "Job", with some basic interface.
class Job
{

public:
    Job(string,string,float);
    ~Job();

    string GetName()
    {
        return Name;
    };

    string GetDescription()
    {
        return Description;
    };

    float GetHours()
    {
        return HoursRequired;
    };

    string DoWork()
    {
        return "My work involves " + Description;
    };


protected:
    string    Name;
    string    Description;
    float     HoursRequired;

};

// "Job" constructor, with variables' initialization.
Job::Job(string name = "Generic job", string desc ="Generic description.", float hrs=40)
{
    Name = name;
    Description = desc;
    HoursRequired = hrs;
}

// "Job" destructor.
Job::~Job()
{
    cout << "[CALLING DESTRUCTOR OF \"" << Name << "\"]\n\n";
}

// Class "Programmer", inheriting from base class "Job".
class Programmer : public Job
{
public:
    Programmer();

};

// "Programmer" constructor.
// This being a derived (and specialized) class, I decided to predetermine some values.
Programmer::Programmer()
{
    Name = "Programmer";
    Description = "Coding, testing, debugging, and deploying software";
    HoursRequired = 80;
}

// Class "Pilot", inheriting from base class "Job".
class Pilot : public Job
{
public:
    Pilot();

};

// "Pilot" constructor.
// This being a derived (and specialized) class, I decided to predetermine some values.
Pilot::Pilot()
{
    Name = "Pilot";
    Description = "Flies in the skies, transports cargo and passengers.";
    HoursRequired = 60;
}

// Template function "IsInBounds".
template <typename T>
void IsInBounds(T value, T lowerBound, T upperBound) {
    if ( (value >= lowerBound) && (value <= upperBound) )
        cout << value << " is in range between " << lowerBound << " and " << upperBound << ".\n";
    else
        cout << value << " is not in range between " << lowerBound << " and " << upperBound << ".\n";

}


// Function "ContainsTheString".
// First parameter is a test function; this can be function reference, or a lambda expression.
// Second parameter is a vector of strings.
int ContainsTheString(function<bool(std::string)> func, vector<std::string> v)
{
    return count_if( v.begin(), v.end(), func);
};

// Test function "first_letter_t_Test".
bool first_letter_t_Test(const std::string& tested)
{
    return tested[0] == 't';
}

// Simple function to test concurrent threads.
// It runs for 10 seconds.
void f1()
{
    cout << "[ENTERING f1()]\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    cout << "[EXITING f1()]\n";
}

// Simple function to test concurrent threads.
// It runs for 5 seconds.
void f2()
{
    cout << "[ENTERING f2()]\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    cout << "[EXITING f2()]\n";
}


// The following declarations are based on the famous example by Bjarne Stroustrup
// about inter-thread communication, and the use of mutex's and condition_variable's
// in order to coordinate sending and receiving messages between processes.
// A Producer and a Consumer objects will run, each on its own thread, using a
// message queue to communicate some information. The message queue is a shared
// resource, so we use a mutex to coordinate safe access to it. A condition
// variable allows the Producer to inform the Consumer once a message is available
// in the queue for processing.

// Class "Message" with the information to be shared by the threads
class Message
{
   public:
       // Constructor
       Message(unsigned int new_message = 0) : m(new_message) {};

       // Interface
       unsigned int getMessage() { return m; };
       void setMessage(unsigned int new_message) { m = new_message; };

   private:
       unsigned int m; // Message

};

queue<Message> mqueue;    // Message queue
condition_variable mcond; // Condition variable communicating events

mutex mmutex;             // Mutex used to lock access to the queue.
                          // This mutex will be used by both "consumer"
                          // and "producer". When a condition variable
                          // starts to "wait" for a particular logic
                          // condition to be fulfilled, the current mutex
                          // is automatically released, so another thread
                          // can use it to safetly access the shared
                          // resources, in this case the message queue.

// Consumer class.
// It waits for mew messages on the message queue from the class Producer
void consumer()
{
    cout << "\n\nCollatz Conjecture sequence, as received from \"producer()\" thread : ";

    while(true)
    {
        unique_lock<mutex> lck{mmutex}; // Acquire mmutex
        mcond.wait(lck, []{ return !mqueue.empty(); }); // Begin to wait for Producer to
                                                        // send a message to the queue.
                                                        // While waiting, the lock is
                                                        // released, so the Mutex can be
                                                        // used by the Producer thread.

        // <-- At this point in the flow, we came out of the wait(), which means we got
        //     a new message in the queue. The lock "lck" is reaquired, and we proceed
        //     to get the message from the queue's front, and to process it.

        auto m = mqueue.front(); // Get the message.
        mqueue.pop();
        lck.unlock(); // Release mmutex

        // Processing the message...
        cout << m.getMessage() << " | ";

        if (m.getMessage() == 1) // Exit condition for infinite loop.
            break;
    }
}

// Producer class.
// It sends messages to the Consumer class thru the message queue.
void producer()
{
    // Providing a seed value for the messages...
	srand((unsigned) time(NULL));
	unsigned int CollatzValue = 1000 + (rand() % 9001); // Random number between 1,000 and 10,000

    while(true)
    {
        Message m(CollatzValue); // Create and fill the message

        unique_lock<mutex> lck{mmutex}; // Acquire mmutex
        mqueue.push(m);                 // Insert new message in the queue
        mcond.notify_one();             // Notify "Consumer" that a new message is available

        if (CollatzValue == 1) // Exit condition for infinite loop.
            break;

        // Calculate the next message's value.
        // According to the Collatz Conjecture, this sequence always converges to 1.
        if ( (CollatzValue % 2) == 0 )
            CollatzValue = CollatzValue / 2;
        else
            CollatzValue = (3*CollatzValue) + 1;

    } // End of scope, mmutex released implicitly
}



// Variadic function Template.
// Base case.
void sentence()
{
  cout << "!\n";
}

// Variadic function Template.
// It takes a variable number of parameters and print them in sequence.
template <typename T, typename... Types>
void sentence(T var1, Types... var2)
{
    cout << var1 << " ";

    sentence(var2...);
}



int main() {

    // Declaration of values used on the function "IsInBounds"
    uint32_t httpResonseCode(544);
    int16_t  int16Bit(10);
    float    pi(3.14);
    double   lighspeed(299792458);
    int      prime(997);

    { // <-- Start of a new block of code, to show the use of smart pointers

        set<std::unique_ptr<Job>> JobCollection; // A Set container with unique_ptr's to the base Class "Job"


        JobCollection.insert( std::unique_ptr<Job>( new Job() ));
        JobCollection.insert( std::unique_ptr<Job>( new Job("Carpenter", "Works with wood,nails and hammers.", 34.5) ));
        JobCollection.insert( std::unique_ptr<Job>( new Pilot ));
        JobCollection.insert( std::unique_ptr<Job>( new Programmer ));
        JobCollection.insert( std::unique_ptr<Job>( new Job("Lawyer", "Works with the law, helping people (sometimes).", 75) ));


        cout << "\nCollection Size : " << JobCollection.size() << "\n\n";

        set<std::unique_ptr<Job>>::iterator itr; // Set iterator.

        // Call the public members of each instance...
        for (itr = JobCollection.begin(); itr != JobCollection.end(); itr++)
          {
            cout << "Name: " << (*itr)->GetName() << "\n";
            cout << "Description: " << (*itr)->GetDescription() << "\n";
            cout << "Hours: " << (*itr)->GetHours() << "\n";
            cout << (*itr)->DoWork() << "\n"
            << "---------------------------------------------------\n";
          }

        cout << "\n\n";


    } //<--- At this point, all smart pointers are destroyed by getting out of scope from this block.

    // Calls to template function "IsInBounds", testing several numeric types...
    IsInBounds<uint32_t>(httpResonseCode, 500, 599);
    IsInBounds<int16_t>(int16Bit, 500, 599);
    IsInBounds<int16_t>(int16Bit, 0, 100);
    IsInBounds<float>(pi, -10, 0);
    IsInBounds<float>(pi, 0, 10);
    IsInBounds<double>(lighspeed, 0, 10);
    IsInBounds<double>(lighspeed, 100000, 1000000);
    IsInBounds<double>(lighspeed, 0, lighspeed);
    IsInBounds<int>(prime, 2, 1000);
    IsInBounds<int>(prime, 1000, 2000);


    // Creating a vector of strings with some values...
    auto theStrings = vector<std::string> {"one", "two", "test", "Mexico", "USA", "France", "auto", "automatic"};

    // Printing the contents of the vector...
    cout << "\n\nThe contents of the vector are:\n";
    for (std::vector<std::string>::const_iterator i = theStrings.begin(); i != theStrings.end(); ++i)
    cout << "\"" << *i << "\"" << " | ";

    // Testing if the string "test" is in the vector.
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    cout << "\n\nVector of strings has " << count << " instance(s) of the value \"test\".\n";

    // Testing if strings different from "test" are in the vector.
    count = ContainsTheString([](const std::string& tested) { return tested != "test"; }, theStrings);
    cout << "\n\nVector of strings has " << count << " instance(s) of the value NOT<\"test\">.\n";

    // Testing if strings with length of 3 are in the vector.
    count = ContainsTheString([](const std::string& tested) { return tested.length() == 3; }, theStrings);
    cout << "\n\nVector of strings has " << count << " instance(s) of length 3.\n";

    // Testing if strings with length of 4 are in the vector.
    count = ContainsTheString([](const std::string& tested) { return tested.length() == 4; }, theStrings);
    cout << "\n\nVector of strings has " << count << " instance(s) of length 4.\n";

    // Testing if strings with the substring "auto" are in the vector.
    count = ContainsTheString([](const std::string& tested) { return tested.find("auto") != std::string::npos; }, theStrings);
    cout << "\n\nVector of strings has " << count << " instance(s) that contain the value \"auto\".\n";

    // Testing if strings that start with the letter 't' are in the vector;
    // this time, we use a function pointer instead of a lambda expression.
    count = ContainsTheString(&first_letter_t_Test, theStrings);
    cout << "\n\nVector of strings has " << count << " instance(s) that start with the value \"t\".\n";

    cout << "\n\n";

    // A simple example of a pair of threads running in parallel...
    std::thread t1 {f1}; // <-- Running for 10 seconds.
    std::thread t2 {f2}; // <-- Running for 5 seconds.

    // Let's wait for the threads to finish before proceeding.
    t1.join();
    t2.join();

    // Example of inter-thread messaging...
    std::thread tConsumer {consumer}; // <-- Consumer thread, it waits for messages.
    std::thread tProducer {producer}; // <-- Producer thread, it sends messages.

    // Let's wait for the threads to finish before proceeding.
    tConsumer.join();
    tProducer.join();

    // Variadic function template examples
    cout << "\n\n";
    sentence(1,1,2,3,5,8,13,21,34,55,89); // Fibonacci sequence

    cout << "\n";
    sentence("Call","me","Ishmael"); // <-- From "Moby Dick"

    cout << "\n";
    sentence("En","un","lugar","de","la","Mancha"); // <-- From "Don Quixote"

    return 0;
}
