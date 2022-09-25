//This was run and tested on https://cpp.sh as I do not have a C++ compiler configured on my personal machine.
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <functional>
#include <cstdlib>
#include <ctime>
#include <sstream>

//Base Job Class and functions
class Job
{
    public:

    Job() = default;
    Job(std::string jobName, int numHours);
    virtual ~Job() = default;

    std::string getName() {return Job::name;}
    std::string getDescription() {return Job::des;}
    uint32_t getNumOfHrs() {return Job::hours;}

    virtual void work();
    int getRandomHour();

    protected:

    std::string name = "";
    std::string des = "";
    uint32_t hours = 0;
};

Job::Job(std::string jobName, int numHours)
{
    name = jobName;
    des = "processing data for " + name;
    hours = numHours;
}

void Job::work() 
{
    std::cout << "My work involves " << des << " over " << hours << " hours." << std::endl;
};

//Programmer class and its constructor
class Programmer : public Job {
    
    public:
    
    Programmer(std::string jobName, int numHours);
    void work() override { Job::work(); }
};

Programmer::Programmer(std::string jobName, int numHours) 
{
    Job::name = jobName;
    Job::des = "processing data for " + Job::name;
    Job::hours = numHours;
};

//Pilot class and its constructor
class Pilot : public Job 
{
    public:

    Pilot(std::string jobName, int numHours);
    void work() override { Job::work(); }
};

Pilot::Pilot(std::string jobName, int numHours) 
{
    Job::name = jobName;
    Job::des = "processing data for " + Job::name;
    Job::hours = numHours;
};

size_t ContainsTheString(std::function<bool(const std::string &)> func, const std::vector<std::string> &strings)
{
    size_t count = 0;

    for(const auto &str : strings)
    {
        count += func(str) == true ? 1 : 0;
    }

    return count;
};

template <class T> void IsInBounds(T input, T left, T right)
{
    std::cout << "Input: " << input << std::endl;
    std::cout << "Range: [" << left << ", " << right << "]" << std::endl;

    if((input > left) && (input < right))
    {
        std::cout << "Input is within range." << std::endl;
    }
    else
    {
        std::cout << "Input is out of bounds." << std::endl;
    }
}

int main()
{
    std::list<Job *> jobsList;
    // Dynamically creating a random amount of jobs between 1 and 20;
    srand(time(0));
    int count = 1 + (rand() % 20);
    std::cout << "Creating " << count << " jobs." << std::endl;

    for(int i = 1; i <= count; i++)
    {
        std::ostringstream s;
        s << "Job" << i;
        Job *job = new Job(s.str(), (rand() % 200));
        jobsList.push_back(job);
    }

    // Dynamically creating a random amount of programmers between 1 and 20
    count = 1 + (rand() % 20);
    std::cout << "Creating " << count << " programmers." << std::endl;

    for(int i = 1; i <= count; i++)
    {
        std::ostringstream s;
        s << "Programmer" << i;
        Programmer *pro = new Programmer(s.str(), (rand() % 200));
        jobsList.push_back(pro);
    }

    // Dynamically creating a randon amount of pilots between 1 and 20
    count = 1 + (rand() % 20);
    std::cout << "Creating " << count << " pilots." << std::endl;

    for(int i = 1; i <= count; i++)
    {
        std::ostringstream s;
        s << "Pilot" << i;
        Pilot *pil = new Pilot(s.str(), (rand() % 200));
        jobsList.push_back(pil);
    }

    // Running all jobs work
    for(auto jobs : jobsList)
    {
        jobs->work();
    }

    // Cleaning up all jobs
    int deleted = 0;
    for(auto job : jobsList)
    {
        deleted++;
        delete job;
        
    }
    std::cout << "Cleaned up " << deleted << " Jobs/Programmers/Pilots." << std::endl;

    uint32_t httpResponse = 555;
    IsInBounds<uint32_t>(httpResponse, 500, 599);

    auto theStrings = std::vector<std::string> {"one", "two", "test"};
    auto strCount = ContainsTheString([](const std::string& tested) {return tested == "test";}, theStrings);
    std::cout << "ContainsTheString: " << int(strCount) << std::endl;

    return 0;
}