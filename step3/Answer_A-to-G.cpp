
	
#include <iostream>
#include <vector>
using namespace std;

class Job
{
public:
    virtual void getDesc() = 0;
    virtual ~Job() {};

    Job(string n, int h,string j)
    {
        _name = n;
        _hours = h;
        _desc = j;
    
    };

    virtual  string getName() { return _name; } ;
    virtual int getHours() { return _hours; };
    virtual void  DoWork() { cout << "My work involves " << _desc.c_str();  return; };  // function that prints out “My work involves ” + the description.
     
protected:
     string _desc;
     string _name;
private:
   
    int _hours;
};

class Pilot : public Job
{
public:
    Pilot(int h) : Job("Pilot", h, "flying an aircraft.") {};
    virtual void getDesc() { cout << _desc; }
};


class Programmer : public Job
{
public:
    Programmer(int h) :Job("Programmer", h, " writting computer programs.") {};
    virtual void getDesc() { cout << _desc; }
};

int main()
{
    vector<Job*> v;

    v.push_back(new Pilot(35));
    v.push_back(new Programmer(40));

    for (vector<Job*>::iterator i = v.begin(); i != v.end(); ++i)
    {
        cout << (*i)->getName() << " - Description : "; 
        (*i)->getDesc();
        cout << " - Working hours: ";
        cout << (*i)->getHours() << " - ";
        (*i)->DoWork();
        cout <<   "\n";

    }

    v.clear();

    return 0;

}	
