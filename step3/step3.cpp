#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>

#define PASS(x) { if ((x) != 0) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; else cout << __FUNCTION__ << " PASSED" << endl; }

#define SUM(x) { if ((x) > 0) std::cout << __FUNCTION__ << " PASSED " << std::endl; else cout << __FUNCTION__ << " FAILED" << endl; }

using namespace std;

class Job
{
    public:
        Job(){
            
        }
		virtual ~Job(){}
		virtual std::string getJobName() = 0;
		virtual std::string getJobDescription() = 0;
		virtual int getJobHours() = 0;
		int DoWork()
		{
		    cout << "My work involves " << getJobDescription() << endl;
		    return 0;
		}
		
};

class Programmer : public Job
{
    public:
    Programmer(){
        
    }
    ~Programmer(){
        
    }
    std::string getJobName() override
    {
        return std::string("Programmer");
    }
    std::string getJobDescription() override
    {
        return std::string("coding and fixing bugs");
    }
    int getJobHours() override
    {
        return 40;
    }
};

class Pilot : public Job
{
    public:
    Pilot(){
        
    }
    ~Pilot(){
        
    }
    std::string getJobName() override
    {
        return std::string("Pilot");
    }
    std::string getJobDescription() override
    {
        return std::string("flying airplanes");
    }
    int getJobHours() override
    {
        return 20;
    }
 
};

template <typename T> T IsInBounds(T num1, T num2, T num3) {
    if (num1 >= num2 && num1 <= num3)
    return 0;
    else
    return 1;
}

auto ContainsTheString(std::function<int(std::string)> funct, std::vector<std::string> theStrings)
{
    int sum = 0;
    for ( auto&& string : theStrings )
    {
        if (funct(string))
        sum++;
    }
    
    return sum;
}

int operations(int index, int op)
{
    std::unique_ptr<Job> array[] =
    {
        std::unique_ptr<Job>(new Programmer),
        std::unique_ptr<Job>(new Pilot)
    };
    
    if(op == 0)
    return (array[index]->getJobName()).empty();
    else if (op == 1)
    return (array[index]->getJobDescription()).empty();
    else if (op == 2)
    return ((array[index]->getJobHours()) < 0);
    else if (op == 3)
    return ((array[index]->DoWork()) != 0);
}

void test_operations()
{
    int x,y = 0;
    for (x=0;x<2;x++)
        for(y=0;y<4;y++)
            PASS(operations(x,y));
    
}

void test_bounds()
{
    PASS(IsInBounds<uint32_t>(515,500,599));
    PASS(IsInBounds<uint32_t>(500,500,599));
    PASS(IsInBounds<uint32_t>(599,500,599));
    PASS(IsInBounds<uint32_t>(607,500,599)); // Expected to fail
    PASS(IsInBounds<uint32_t>(525,500,599));
    PASS(IsInBounds<uint32_t>(415,500,599)); // Expected to fail
}

void test_strings()
{   
    auto theStrings = vector<std::string> { "one", "test",  "test"};
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    SUM(count);
    theStrings = vector<std::string> { "test", "test",  "test"};
    count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    SUM(count);
}

int main() {
    
    std::unique_ptr<Job> array[] =
    {
        std::unique_ptr<Job>(new Programmer),
        std::unique_ptr<Job>(new Pilot)
    };
    
    
    for( auto&& pointer : array )
    {
        pointer->getJobName();
        pointer->getJobDescription();
        pointer->getJobHours();
        pointer->DoWork();
    }
    
    uint32_t httpResponseCode = 410;
    cout << "Function returned " << IsInBounds<uint32_t>(httpResponseCode, 500, 599) << endl;
   
    auto theStrings = vector<std::string> { "one", "two",  "test"};
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
   
    cout << "There is " << count << " test strings in the collection" << endl;
   
    test_operations();
    test_bounds();
    test_strings();
    
    return 0;
}
