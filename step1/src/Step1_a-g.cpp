/*
* Step1_a-g.cpp
* This file covers steps 'a' to 'g' of 'Requirements.txt'
* There is a unit test for memory leak
*/

#include <string>
#include <ctime>
#include <list>
#include <iostream>
#include <vector>
#include <cassert>

class Job
{
protected:

    std::string name;
    std::string description;
    int requiredHours;

public:

    virtual std::string GetName() = 0;

    virtual std::string GetDescription() = 0;

    virtual int GetRequiredHours() = 0;

    void DoWork() {
        std::cout << "My work involves " << GetDescription() << std::endl;
    };
};

class Programmer :
    public Job
{
private:
    std::string language;
public:
    Programmer(std::string name, std::string description, int requiredHours, std::string language)
    {
        this->name = name;
        this->description = description;
        this->requiredHours = requiredHours;
        this->language = language;
    }

    std::string GetName()
    {
        return name;
    }

    // Changing to be different in derived classes
    std::string GetDescription()
    {
        return description + " in " + language;
    }

    int GetRequiredHours()
    {
        return requiredHours;
    }
};


class Pilot :
    public Job
{
public:
    Pilot(std::string name, std::string description, int requiredHours)
    {
        this->name = name;
        this->description = description;
        this->requiredHours = requiredHours;
    }

    std::string GetName()
    {
        return name;
    }

    std::string GetDescription()
    {
        return description;
    }

    // Changing to be different in derived classes
    int GetRequiredHours()
    {
        struct tm newtime;
        time_t now = time(0);
        localtime_s(&newtime, &now);

        if (newtime.tm_wday == 0) //Sunday requires one hour less
        {
            return requiredHours - 1;
        }

        return requiredHours;
    }
};

// Unit test memory leak
void onTestMemory_ShouldNotLeak()
{
    assert(_CrtDumpMemoryLeaks() == 0);
}

void useJobInterface()
{
    Programmer* programmer = new Programmer(std::string("programmer"), std::string("programming"), 8, std::string("C++"));
    Pilot* pilot = new Pilot(std::string("pilot"), std::string("piloting"), 10);

    std::vector<Job*> vector = { programmer , pilot };
    std::vector<Job*>::iterator it;

    for (it = vector.begin(); it != vector.end(); it++)
    {
        std::cout << (*it)->GetName() << std::endl;
        std::cout << (*it)->GetDescription() << std::endl;
        std::cout << (*it)->GetRequiredHours() << std::endl;
        (*it)->DoWork();
    }

    delete programmer;
    delete pilot;
}

int main()
{
    
    useJobInterface();
    onTestMemory_ShouldNotLeak();

    return 0;
}