#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

////STRUCTURES START
class Job
{
public:
    // constructor
    Job() {}

    virtual // Ensure to invoke actual object destructor
        ~Job()
    {
    }

    // interface methods
    string DisplayTitle()
    {
        return m_job_name;
    }
    string Describe()
    {
        return m_job_description;
    }
    int WorkWeek()
    {
        return m_job_hours;
    }
    void DoWork()
    {
        cout << "My work involves " << Describe() << endl;
    }

protected:
    string m_job_name;
    string m_job_description;
    int m_job_hours;
};

class Programmer : public Job
{
public:
    Programmer()
    {
        m_job_name = "Programmer";
        m_job_description = "designing and creating software";
        m_job_hours = 40;
    }

    ~Programmer() {}
};

class Pilot : public Job
{
public:
    Pilot()
    {
        m_job_name = "Pilot";
        m_job_description = "flying and not crashing aircraft";
        m_job_hours = 50;
    }

    ~Pilot() {}
};
////STRUCTURES END

////TEMPLATE FUNCTIONS START
template <class T>
bool IsInBounds(T arg, T low_range, T high_range)
{
    if ((low_range < arg) && (arg < high_range))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Variadic template
template <typename T, typename... Targs>
void coutAll(T term1, Targs... nextTerm)
{
    cout << term1;
    coutAll(nextTerm...);
}

////TEMPLATE FUNCTIONS END

////UTILITY FUNCTIONS START
int ContainsTheString(bool (*testFunction)(const string &), vector<string> testdata)
{
    int result(0);
    for (auto i : testdata)
    {
        if (testFunction(i))
        { // we have a match!
            ++result;
        }
    }
    return result;
}
////UTILITY FUNCTIONS END

int main()
{
    // dynamic allocation
    Job *prg_job = new Programmer();
    Job *plt_job = new Pilot();
    // my job collection
    Job *all_jobs[] = {prg_job, plt_job};

    for (auto i : all_jobs)
    {
        coutAll("Position: ", i->DisplayTitle(), "\n", "Description: ", i->Describe(), "\n", "Hours per week needed: ", i->WorkWeek());
        i->DoWork();
        cout << endl; // space out each entry
    }

    // Test template function
    uint32_t httpResponseCode = 570;
    bool test_bounds_result = IsInBounds<uint32_t>(httpResponseCode, 500, 599);
    cout << "Testing of IsInBounds for " << httpResponseCode << " within the bounds of 500 and 599 results in " << ((test_bounds_result) ? "IN BOUNDS" : "OUT OF BOUNDS") << endl;

    // Strings vector test
    auto theStrings = vector<std::string>{"one", "two", "test"};
    auto count = ContainsTheString([](const std::string &tested)
                                   { return tested == "test"; },
                                   theStrings);
    cout << "Testing of theStrings vector has resulted in " << count << " match(es)!" << endl;

    // memory cleanup
    delete prg_job;
    delete plt_job;
}