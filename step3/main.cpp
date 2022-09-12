// Example program
// The requirements from a to k have been fulfilled
// in this program.

#include <iostream>
#include <string>
#include <list>
#include <vector>

class Job
{

    private:
    std::string nameOfJob = "";
    std::string descriptionOfJob = "";
    uint32_t hoursOfJob = 0;
    
    public:
    Job(std::string jobName)   ;
    ~Job()  ;
    static std::list<Job*> jobList;    
    std::string GetNameOfJob();
    std::string GetDescriptionOfJob();
    uint32_t GetNumberOfHoursOfJob();
    
    void DoWork();
    void CallInterfaceFunctions();
       
};

std::list<Job*> Job::jobList;

Job::Job(std::string jobName)
{
    nameOfJob = jobName;
    jobList.push_back(this);
};

Job::~Job()
{
    std::list<Job*>::iterator pointer;
    pointer = find(jobList.begin(), jobList.end(), this);
    if (pointer != jobList.end())
    {
        jobList.erase(pointer);
    }
};

void Job::CallInterfaceFunctions()
{
    for (std::list<Job*>::iterator itor = jobList.begin(); itor !=jobList.end(); ++itor)
    {
        (*itor)->GetNameOfJob();
        (*itor)->GetDescriptionOfJob();
        (*itor)->GetNumberOfHoursOfJob();
        (*itor)->DoWork();
    }
};

void Job::DoWork()
{
    std::cout << "My work involves starting the " << nameOfJob << ".\n" << std::endl;
}

std::string Job::GetNameOfJob()
{
    return Job::nameOfJob;
}

std::string Job::GetDescriptionOfJob()
{
    return Job::descriptionOfJob;
}

uint32_t Job::GetNumberOfHoursOfJob()
{
    return Job::hoursOfJob;
}
    
class Programmer : public Job
{
};

class Pilot : public Job
{
};

template <class T> void IsInBounds(T inputNumber, T leftBound, T rightBound)
{
    std::cout << "Input number is " << inputNumber << std::endl;
    std::cout << "Range is [" << leftBound << ", " << rightBound << "]" << std::endl;
    if ((inputNumber > leftBound) && (inputNumber < rightBound))
    {
        std::cout << "It is within the range\n" << std::endl;
    }
    else
    {
        std::cout << "It is out of the range\n" << std::endl;
    }
};

void testStringMatch(std::vector<std::string> inputVector, std::string testString)
{
    size_t matchCount = 0;
    for (size_t i = 0; i < inputVector.size(); ++i)
    {
        if (inputVector[i] == testString)
        matchCount++;
    }
    std::cout << "The number of match is " << matchCount << std::endl;
}

template<typename T>
T sum(T v)
{
  return v;
}

template<typename T, typename... Args>
T sum(T first, Args... args)
{
  return first + sum(args...);
};

int main()
{
  
  Job *job1 = new Job("Job1");
  job1->CallInterfaceFunctions();
  Job *job2 = new Job("Job2");
  job2->CallInterfaceFunctions();
  delete job1;
  delete job2;
  
  
  uint32_t httpResponse = 555;
  IsInBounds<uint32_t>(httpResponse, 500, 599);
  
  std::vector<std::string> stringVector = {"one", "two", "test"};
  std::string testString = "test";
  testStringMatch(stringVector, testString);

  // Variadic Template Usage  
  uint32_t sumResult = sum(1, 2, 3, 4, 5);
  std::cout << "The added sum is " << sumResult << std::endl;
  std::string s1 = "A";
  std::string s2 = "B";
  std::string s3 = "C";
  std::string s4 = "D";
  std::string s5 = "E";
  std::string stringResult = sum(s1, s2, s3, s4, s5);
  std::cout << "The added string is " << stringResult << std::endl;
  
  return 0;
}
