// Online C++ compiler to run C++ program online
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

//Job Abstract Class
class Job {
    public:
    Job(){}
    virtual std::string getName() = 0;
    virtual std::string getDescription() = 0;
    virtual int getRequiredHours() = 0;
    virtual void DoWork() = 0;
    virtual ~Job(){};
    
    protected:
    std::string m_name{"Job"};
    std::string m_description{"This is my Job"};
    int m_requiredHours{8};
};

//Programmer Job's child sub class
class Programmer: public Job {
    
    public:
    Programmer(){
        m_name = "Programmer";
        m_description = " doing my Programmer Job";
        m_requiredHours = 8;
    }
    ~Programmer(){}
    
    std::string getName()  {return m_name;}
    std::string getDescription()  {return m_description;}
    int getRequiredHours()  {return m_requiredHours;}
    void DoWork()  {
        std::cout<<"My work involves" + m_description;
    }

};
int main() {

    return 0;
}