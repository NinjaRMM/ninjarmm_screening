// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<string.h>
#include <list>

using namespace std;

class Job
{
protected:
    string name;
    string description;
    int NumberofHours = 0;
    public:
        void Instantiate(string Name, string Description, int NumberofHours)
        {
            this->name = Name;
            this->description = Description;
            this->NumberofHours = NumberofHours;
        }
        virtual string getName()
        {
            return this->name;
        }
        virtual string getDescription()
        {
            return this->description;
        }
        virtual int getNumberofHoursRequired()
        {
            return this->NumberofHours;
        }
        virtual string DoWork()
        {
            return "My work involves " + this->description;
        }
};

class Programmer : public Job 
{
};

class Pilot : public Job 
{
};

template <typename T>
T Check(T num1)
{
    if (num1 == 200) return true;
    else return false;
}

void write()
{
    cout << "I am empty and called at the end.";
}

template <typename T, typename ... Types>
void write(T var1, Types... var2)
{
    cout << var1 << endl;
    write(var2...);
}

int main()
{
    Job job1;
    Programmer job2;
    Pilot job3;    
    int response;
    bool result;
    job1.Instantiate("Janitor", "Janitorial Job", 8);
    job2.Instantiate("Programmer", "IT", 10);
    job3.Instantiate("Pilot", "Touring", 12);
    Job jobs[3] = {job1, job2, job3};
    for (int i = 0; i < 3; i++)
    {
        cout << jobs[i].getName() << endl;
        cout << jobs[i].DoWork() << endl;
    }    
    cout << "Enter A value: " << endl;
    cin >> response;
    result = Check(response);
    if (result == true) cout << "true" << endl;
    else cout << "false" << endl;
    // variadic template serves as answer for I :) peace
    write(1, 2, 3, 4, 5, 6);
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
