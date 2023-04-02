
class Job
{
public:
    const char * getJobName();
    const char * getJobDescription();
    int getJobHours();
    void DoWork();

protected:
    const char * jobName;
    const char * jobDescription;
    int jobHours;
};


class Programmer : public Job
{
public:
    Programmer(){
        jobName = "Programmer";
        jobDescription = "write, test, and maintain computer programs using a programming language.";
        jobHours = 100;
    };

};

class Pilot : public Job
{
public:
    Pilot(){
        jobName = "Pilot";
        jobDescription = "operate and navigate an aircraft safely from one destination to another.";
        jobHours = 10;
    };
};
