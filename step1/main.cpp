#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
using namespace std;

template<class T>
void FailIfDifferent(const T &value1, const T &value2)
{
    if(value1!=value2)
    {
        throw std::runtime_error("Values are different and equal values were expected");
    }
}
template<class T>
void FailIfEqual(const T &value1, const T &value2)
{
    if(value1==value2)
    {
        throw std::runtime_error("Values are equal, different values were expected");
    }
}

class Job
{
public:
    virtual ~Job() = default;
    virtual void eat() = 0;
    virtual void sleep() = 0;
    virtual void rest() = 0;
    virtual void work() = 0;
};

class Pilot : public Job
{
public:
    void eat() override
    {
        cout<<"Pilot is eating"<<endl;
    }
    void sleep() override
    {
        cout<<"Pilot is sleeping"<<endl;
    }
    void rest() override
    {
        cout<<"Pilot is resting"<<endl;
    }
    void work() override
    {
        cout<<"Pilot is working"<<endl;
    }
};

class Programmer : public Job
{
public:
    void eat() override
    {
        cout<<"Programmer is eating"<<endl;
    }
    void sleep() override
    {
        cout<<"Programmer is sleeping"<<endl;
    }
    void rest() override
    {
        cout<<"Programmer is resting"<<endl;
    }
    void work() override
    {
        cout<<"Programmer is working"<<endl;
    }
};

template<class T>
unique_ptr<T> buildJob()
{
    return unique_ptr<T>(new T());
}

template<typename T>
bool IsInBounds(T element, T minValue, T maxValue)
{
    return element >= minValue && element <= maxValue;
}

unsigned int ContainsTheString(function<bool(const string&)> func, const vector<string>& collection)
{
    unsigned int result = 0;
    for (const string& elem : collection)
    {
        result += func(elem);
    }
    return result;
}

template<typename T, typename U>
T divide(T firstValue, U secondValue)
{
    T result{};
    if(!is_same<T,U>::value)
    {
        result=firstValue/static_cast<T>(secondValue);
    }
    else
    {
        result=firstValue/secondValue;
    }
    return result;
}

void TestContainsTheString()
{
    vector<string> theStrings = { "one", "two", "three"};
    FailIfEqual<unsigned int>(ContainsTheString([](const string& tested) { return tested == "two"; }, theStrings), 0);
}

void TestDivideIsDouble()
{
    FailIfDifferent(divide(1.0, 2), 0.5);
}

void TestDivideIsInt()
{
    FailIfDifferent(divide(2, 2.5), 1);
}

void TestIsInBoundsNotIn()
{
    uint32_t httpResonseCode = 400;
    FailIfDifferent(IsInBounds<uint32_t>(httpResonseCode, 500, 599), false);
}

void TestIsInBounds()
{
    uint32_t httpResonseCode = 501;
    FailIfDifferent(IsInBounds<uint32_t>(httpResonseCode, 500, 599), true);
}

void runTests()
{
    std::map<std::string,std::function<void()>> testVsMethods;
    testVsMethods["ContainsTheString"] = bind(&TestContainsTheString);
    testVsMethods["DivideIsDouble"] = bind(&TestDivideIsDouble);
    testVsMethods["DivideIsInt"] = bind(&TestDivideIsInt);
    testVsMethods["IsInBounds"] = bind(&TestIsInBounds);
    testVsMethods["IsInBoundsNotIn"] = bind(&TestIsInBoundsNotIn);
    std::map<std::string, std::function<void()>>::iterator it;
    for(it = testVsMethods.begin(); it != testVsMethods.end(); ++it)
    {
        std::cout << "Running " << it->first << "\t";
        try
        {
            it->second();
            std::cout << "[PASSED]" << std::endl;
        }
        catch(std::exception& ex)
        {
            std::cout << "[FAIL] Test " << it->first << "failed: " << ex.what() << std::endl;
        }
        catch(...)
        {
            std::cout << "[FAIL] Test " << it->first << "failed due to an unknown exception " << std::endl;
        }
    }

}

int main(int argc, char* argv[])
{
    if(argc==2 && string(argv[1])=="test")
    {
        runTests();
    }
    vector<unique_ptr<Job>> jobContainer;
    jobContainer.push_back(buildJob<Pilot>());
    jobContainer.push_back(buildJob<Programmer>());
    for(auto&& elem : jobContainer)
    {
        elem->eat();
        elem->sleep();
        elem->rest();
        elem->work();
    }
    uint32_t httpResonseCode = 400;
    if (!IsInBounds<uint32_t>(httpResonseCode, 500, 599))
    {
        cout << "The value provided is not within bounds" << endl;
    }
    httpResonseCode = 550;
    if (IsInBounds<uint32_t>(httpResonseCode, 500, 599))
    {
        cout << "The value provided is within bounds" << endl;
    }
    vector<string> theStrings = { "one", "two", "test"};
    auto count = ContainsTheString([](const string& tested) { return tested == "test"; }, theStrings);
    cout << count << endl;
    cout << divide(1.0, 2) <<endl;
    cout << divide(2, 2.5) <<endl;
    return 0;
}
