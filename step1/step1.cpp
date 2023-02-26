/*******************************************************************************************************
**                                         File information
** File name:           step1.cpp
** Description:         Code from step1 with the unit tests.
**------------------------------------------------------------------------------------------------------
** Created by:          Marlon Zanardi <marlon.zanardi95@hotmail.com>
** Creation date:       Feb 22, 2023    
********************************************************************************************************/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <vector>

using namespace std;

class Job
{
    public:
        Job(string name, string description, int hoursRequired) : m_name(name), m_description(description), m_hoursRequired(hoursRequired) {}
        virtual ~Job() {}
        string getName() const
        { 
            return m_name; 
        }
        string getDescription() const
        { 
            return m_description; 
        }
        int getHoursRequired() const 
        { 
            return m_hoursRequired; 
        }
        virtual void doWork() const
        {
            cout << "My work is " << m_description << endl;
        }
    private:
        string m_name;
        string m_description;
        int m_hoursRequired;
};

class Programmer : public Job
{
    public:
        Programmer(string name, string description, int hoursRequired, string language) : Job(name, description, hoursRequired), m_language(language) {}
        virtual ~Programmer() {}
        string getLanguage() const 
        { 
            return m_language; 
        }
        virtual void doWork() const
        {
            cout << "My work is programming in " << m_language << endl;
        }
    private:
        string m_language;
    };

class Pilot : public Job
{
    public:
        Pilot(string name, string description, int hoursRequired, string aircraftType) : Job(name, description, hoursRequired), m_aircraftType(aircraftType) {}
        virtual ~Pilot() {}
        string getAircraftType() const
        { 
            return m_aircraftType;
        }
        virtual void doWork() const
        {
            cout << "My work is flying " << m_aircraftType << endl;
        }
    private:
        string m_aircraftType;
};

template<typename T>
bool isInBounds(T value, T lower, T upper)
{
    bool answer = false;
    if (value >= lower && value <= upper)
    {
        answer = true;
    }
    return answer;
}

template<typename Func>
int countMatching(Func testFunc, const vector<string>& collection)
{
    int count = 0;
    for (const auto& item : collection)
    {
        if (testFunc(item))
        {
            count++;
        }
    }
    return count;
}

int main_func()
{
    // Create some instances of Job, Programmer, and Pilot
    Job* j1 = new Job("Tester", "Test the code", 8);
    Programmer* p1 = new Programmer("Developer", "Writing code", 40, "C++");
    Pilot* pl1 = new Pilot("Captain", "Flying passengers", 12, "Boeing 777");

    // Add them to a vector
    vector<Job*> jobs = { j1, p1, pl1 };

    // Iterate through the vector and call the interface functions
    for (const auto& job : jobs)
    {
        cout << "Name: " << job->getName() << endl;
        cout << "Description: " << job->getDescription() << endl;
        cout << "Hours required: " << job->getHoursRequired() << endl;
        job->doWork();
        cout << endl;
    }

    // Test the isInBounds function
    uint32_t httpResponseCode = 404;
    bool isInRange = isInBounds<uint32_t>(httpResponseCode, 500, 599);
    cout << "Is " << httpResponseCode << " within the range 500-599? ";
    string result = isInRange ? "Yes" : "False";
    cout << "Response: " << result << endl << endl;

    // Test the countMatching function
    vector<string> theStrings = { "one", "two", "test" };
    auto count = countMatching([](const std::string& tested){ return tested == "test"; }, theStrings);
    cout << "Number of strings in the collection that match 'test': " << count << endl;

    // Clean up the dynamically allocated objects
    for (const auto& job : jobs)
    {
        delete job;
    }

    return 0;
}

TEST_CASE("Job class functions", "[Job]")
{
    Job j1("Tester", "Test the code", 8);
    SECTION("getName() returns the correct name")
    {
        REQUIRE(j1.getName() == "Tester");
    }
    SECTION("getDescription() returns the correct description")
    {
        REQUIRE(j1.getDescription() == "Test the code");
    }
    SECTION("getHoursRequired() returns the correct hours required")
    {
        REQUIRE(j1.getHoursRequired() == 8);
    }
}

TEST_CASE("Job getters return expected values", "[Job]")
{
    Job j("Tester", "Test the code", 8);

    SECTION("getName returns correct value") {
        REQUIRE(j.getName() == "Tester");
    }

    SECTION("getDescription returns correct value") {
        REQUIRE(j.getDescription() == "Test the code");
    }

    SECTION("getHoursRequired returns correct value") {
        REQUIRE(j.getHoursRequired() == 8);
    }
}

TEST_CASE("countMatching function", "[countMatching]") {
    vector<string> theStrings = { "one", "two", "test", "test" };
    SECTION("count of matches is correct") {
        auto count = countMatching([](const std::string& tested){ return tested == "test"; }, theStrings);
        REQUIRE(count == 2);
    }
}