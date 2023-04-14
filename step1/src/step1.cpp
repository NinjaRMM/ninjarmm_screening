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
        : Job{ name, descr, hrsReq } // Call Job(name, desc, hrsReq) constructor
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
        : Job{ name, descr, hrsReq } // Call Job(name, desc, hrsReq) constructor
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
	i. Create a function that will take a vector of strings and a test function, It should return how many 
	   items in the collection matched the test. Then print out the result.
		i. auto theStrings = vector<std::string> { “one”, “two”,  “test”};
		ii. auto count = ContainsTheString([](const std::string& tested) { return tested == “test” }, theStrings);
	j. Write any other code you feel would show off your understanding of C/C++ and 
	   programming in general. Bonus: Variadic Template /w usage. 
	k. Please add passing unit tests. 
*/

// Create the vector of strings to use for testing
auto theStrings = vector<std::string> { "one", "two", "test"};

//auto count = ContainsTheString([](const std::string& tested) { return tested == "test" }, theStrings);

//int ContainsTheString( [](const std::string &tested)->bool, vector<std::string>);
// lambda [](const std::string &tested)->bool

// testing approaches
template <typename F>
void use_func(const F& func) {
    const char * name = "use_func";
    func(name);
}

int main()
{

    // testing approaches
    use_func([](const char* callee) {
        cout << "This lambda is invoked from " << callee << endl;
    });
    cout << endl << endl << endl;





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