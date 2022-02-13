#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

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

template <class T>
void IsInBounds(T arg, T low_range, T high_range)
{
    cout << "Input : " << arg;
    if ((low_range < arg) && (arg < high_range))
    {
        cout << " is between ";
    }
    else
    {
        cout << " is not between ";
    }
    cout << low_range << " and " << high_range << endl;
}
////TEMPLATE FUNCTIONS START

////TEMPLATE FUNCTIONS END
int main()
{
    // dynamic allocation
    Job *prg_job = new Programmer();
    Job *plt_job = new Pilot();
    Job *all_jobs[] = {prg_job, plt_job};

    for (auto i : all_jobs)
    {
        cout << "Position: " << i->DisplayTitle() << endl;
        cout << "Description: " << i->Describe() << endl;
        cout << "Hours per week needed: " << i->WorkWeek() << endl;
        i->DoWork();
        cout << endl; // space out each entry
    }

    // Test template function
    uint32_t httpResponseCode = 570;
    IsInBounds<uint32_t>(httpResponseCode, 500, 599);

    // memory cleanup
    delete prg_job;
    delete plt_job;
}