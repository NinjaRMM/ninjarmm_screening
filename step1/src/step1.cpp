#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <functional>
using namespace std;

class Job
{
private:
    string m_name;
    string m_descr;
    int    m_hrsReq {};

public:
    Job(string name, string descr, int hrsReq=0)
        : m_name {name},
        m_descr {descr},
        m_hrsReq{hrsReq}
    {
    }

    string getName() const { return m_name; }
    string getDescr() const { return m_descr; }
    int getHrsReq() const { return m_hrsReq; }
    void DoWork() const {cout << "My work involves " << m_descr;}
    virtual void getInfo() const {cout << "Name:" << m_name << " Descr:" << m_descr << " Hrs_Req:" << m_hrsReq << " "; DoWork();}
};

class Programmer: public Job
{
private:
    int m_nLang {};

public:
    Programmer(string name, string descr, int hrsReq, int nLang)
        : Job{ name, descr, hrsReq }
        , m_nLang{ nLang }
    {
    }

    int getNumLanguages() const { return m_nLang; }
    void getInfo() const
    {
        Job::getInfo();
        cout << " NumLang:" << m_nLang; 
    }
};

class Pilot: public Job
{
private:
    int m_hrsFlown {};

public:
    Pilot(string name, string descr, int hrsReq, int hrsFlown)
        : Job{ name, descr, hrsReq }
        , m_hrsFlown{ hrsFlown }
    {
    }

    int getHrsFlown() const { return m_hrsFlown; }
    void getInfo() const
    {
        Job::getInfo();
        cout << " HrsFlown:" << m_hrsFlown; 
    }
};

template <class T>
bool isInBounds(T input, T lower, T upper)
{
    bool result = (input >= lower && input <= upper);
    return result;
}


/*
	j. Write any other code you feel would show off your understanding of C/C++ and 
	   programming in general. Bonus: Variadic Template /w usage. 
	k. Please add passing unit tests. 
*/

// Function to count number of matches of test function in string vector
template <typename F>
int ContainsTheString( const F& func, vector<std::string> strings)
{
    int nMatch = 0;

    // Iterate over the vector of strings
    for (const auto& theStr: strings)
    {
        // If the string matches the test function, increment the count
        if( func(theStr) ) ++nMatch;
    }
    return nMatch;
}


int main()
{
    // Create the vector of strings to use for testing
    auto theStrings = vector<std::string> { "test", "one", "two", "test"};

    auto numMatches = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    cout << "\'ContainsTheString\' function found " << numMatches << " matches." << endl << endl;

    // Use reference_wrapper to avoid object slicing
    vector<reference_wrapper<Job>> jobs{};

    // Dynamically allocate Job and move to vector
    Job * j = new Job{ "Job", "Base Job", 8};
    jobs.push_back(*j);
 
    // Dynamically allocate Pilot and move to vector
    Pilot * p = new Pilot{ "Pilot", "Fly planes", 10, 345 };
    jobs.push_back( *p );

    // Dynamically allocate Programmer and move to vector
    Programmer * pr = new Programmer{ "Programmer", "C++ Programming", 12, 8 };
    jobs.push_back( *pr );

    // Iterate over vector and display object info
    // Note that each obj type has different members to display
    for (const auto& job: jobs)
    {
        job.get().getInfo(); cout << endl;
        assert(job.get().getHrsReq() >= 1); 
    }

    cout << "\nTest Template Function: isInBounds<uint32_t>(httpResonseCode, 500, 599):" << endl;
    vector<uint32_t> v {499, 500, 501, 555, 598, 599, 600};
    for( int i=0; i < v.size(); i++ )
    {
        bool result = isInBounds<uint32_t>(v[i], 500, 599);
        string sRes = (result) ? "true" : "false";
        cout << "isInBounds<uint32_t>(" << v[i] << ", 500, 599) => " << sRes << endl; 
    }
    return 0;
}