#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

class Job
{
public:
    virtual string get_name() const = 0;
    virtual string get_description() const = 0;
    virtual uint32_t get_num_hours() const = 0;
    virtual void DoWork() const
    {
        cout << "My work involves " << description << endl;
    }
    virtual ~Job() {}

protected:
    Job(const string &name, const string &description, uint32_t num_hours)
        : name(name), description(description), num_hours(num_hours) {}
    string name;
    string description;
    uint32_t num_hours;
};

class Programmer : public Job
{
public:
    Programmer(const string &name, const string &description, uint32_t num_hours) : Job(name, description, num_hours) {}

    string get_name() const
    {
        return name;
    }
    string get_description() const
    {
        return description;
    }
    uint32_t get_num_hours() const
    {
        return num_hours;
    };
    virtual ~Programmer() {}
};

class Pilot : public Job
{
public:
    Pilot(const string &name, const string &description, uint32_t num_hours) : Job(name, description, num_hours) {}

    string get_name() const
    {
        return name;
    }
    string get_description() const
    {
        return description;
    }
    uint32_t get_num_hours() const
    {
        return num_hours;
    };
    virtual ~Pilot() {}
};

template <typename T>
bool IsInBounds(T code, uint32_t lower_bound, uint32_t upper_bound)
{
    return code >= lower_bound && code <= upper_bound;
}

uint32_t ContainsTheString(function<bool(const string &)> fn, const vector<string> &strs)
{
    return count_if(strs.begin(), strs.end(), fn);
}

int main()
{
    vector<Job *> jobs;
    jobs.push_back(new Programmer("Programmer", "coding", 50));
    jobs.push_back(new Pilot("Pilot", "flying", 60));
    for (const auto &job : jobs)
    {
        cout << "Name: " << job->get_name() << endl;
        cout << "Description: " << job->get_description() << endl;
        cout << "Num hours: " << job->get_num_hours() << endl;
        job->DoWork();
    }

    for (const auto &job : jobs)
    {
        delete job;
    }

    uint32_t httpResponseCode = 550;
    cout << httpResponseCode << " is within bounds: " << IsInBounds(httpResponseCode, 500, 599) << endl;

    auto theStrings = vector<std::string>{"one", "two", "test"};
    auto count = ContainsTheString([](const std::string &tested)
                                   { return tested == "test"; },
                                   theStrings);
    cout << "Num matches: " << count << endl;
    return 0;
}