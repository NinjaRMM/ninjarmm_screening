
class Job
{
public:
    const char * getJobName();
    const char * getJobDescription();
    int getJobHours();
    void DoWork();

protected:
    const char * _name;
    const char * _description;
    int _hours;
};


class Programmer : public Job
{
public:
    Programmer(){
        _name = "Programmer";
        _description = "write, test, and maintain computer programs using a programming language.";
        _hours = 100;
    };

};

class Pilot : public Job
{
public:
    Pilot(){
        Job();
        // _name = "Pilot";
        // _description = "operate and navigate an aircraft safely from one destination to another.";
        // _hours = 10;
    };
};
