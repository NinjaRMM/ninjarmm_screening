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
int main() {

    return 0;
}