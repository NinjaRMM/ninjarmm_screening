
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <initializer_list>


// ------------------------------------------------
// -------------------- Part 1 --------------------
// ------------------------------------------------

class Job 
{

public:
    virtual std::string GetName() = 0;
    virtual std::string GetDescription() = 0;
    virtual int GetHoursRequired() = 0;
    void DoWork()
    {
        std::cout << "My work involves " + GetDescription() << std::endl; 
    }
    
    virtual ~Job() {}

};

class Programmer : public Job
{
public:
    std::string GetName() override 
    {
        return "Programmer";
    }

    std::string GetDescription() override 
    {
        return "designing and building software.";
    }

    int GetHoursRequired() override 
    {
        return 3000;
    }
};

class Pilot : public Job 
{
public:
    std::string GetName() override 
    {
        return "Pilot";
    }

    std::string GetDescription() override 
    {
        return "piloting aircrafts.";
    }

    int GetHoursRequired() override 
    {
        return 5000;
    }
};

//Part 1 corresponds to steps a to g 
void DoPart1() 
{
    std::cout << "--- 1 ---" << std::endl;
    std::vector<std::unique_ptr<Job>> jobs;
    jobs.push_back(std::make_unique<Programmer>());
    jobs.push_back(std::make_unique<Pilot>());
    jobs.push_back(std::make_unique<Programmer>());
    jobs.push_back(std::make_unique<Pilot>());
    
    for(size_t i = 0; i < jobs.size(); i++)
    {
        std::cout << jobs[i]->GetName() << ": " << jobs[i]->GetDescription() << " Requires: " << jobs[i]->GetHoursRequired() << " hours. ";
        jobs[i]->DoWork(); 
    }
}



// ------------------------------------------------
// -------------------- Part 2 --------------------
// ------------------------------------------------


template <typename T>
bool IsInBounds(T value, T minVal, T maxVal)
{
    return value >= minVal && value <= maxVal;
} 

//Part 2 corresponds to step h
void DoPart2()
{
    std::cout << "--- 2 ---" << std::endl;
    uint32_t httpResonseCode = 503;
    uint32_t minVal = 500;
    uint32_t maxVal = 599;
    std::cout << "IsInBounds. Value: " << httpResonseCode << ", Min: " << minVal << ", Max: " << maxVal << " -> " << (IsInBounds<uint32_t>(httpResonseCode, minVal, maxVal) ? "True" : "False") << std::endl;
}


// ------------------------------------------------
// -------------------- Part 3 --------------------
// ------------------------------------------------


size_t ContainsTheString(std::function<bool(const std::string&)> testFunc, const std::vector<std::string>& strings)
{
    if(!testFunc)
        return 0;

    size_t count = 0;
    for(size_t i = 0; i < strings.size(); i++)
    {
        if(testFunc(strings[i]))
            count++;
    }

    return count;
}

//Part 3 corresponds to step i
void DoPart3()
{
    std::cout << "--- 3 ---" << std::endl;
    auto theStrings = std::vector<std::string> { "one", "two", "test"};
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    std::cout << "Matched strings: " << count << std::endl;
}


// ------------------------------------------------
// -------------------- Part 4 --------------------
// ------------------------------------------------

/* 
StackVector class is a vector class that gets allocated to the stack instead of heap.
In some use cases this can avoid the overhead of dynamically allocating the array.
If the macro STACK_VECTOR_DISABLE_CHECKS is defined, all safety checks are disabled.
*/
template <typename T, size_t MaxSize>
class StackVector
{

    void fill(const std::initializer_list<T>& l)
    {
        count = std::min(l.size(), MaxSize);

        size_t i = 0;

        for(const auto& v : l)
        {
            if(static_cast<int>(i) > static_cast<int>(count) - 1)
                break;

            d[i] = v;
        }
    }

public:
    StackVector() = default;
    
    StackVector(std::initializer_list<T> l)
    {
        fill(l);
    }

    template<typename... Args>
    StackVector(const Args&... args)
    {
        fill({args...});
    }

    StackVector& operator=(const StackVector& other)
    {
        count = other.count;
        std::memcpy(d, other.d, count * sizeof(T));
        return *this;
    }

    T& operator[](const size_t& i) 
    { 
        #ifndef STACK_VECTOR_DISABLE_CHECKS
        if(count == 0 || i > count - 1)
            throw std::out_of_range("Accessing element out of range.");
        #endif

        return d[i]; 
    }

    const T& operator[](const size_t& i) const 
    {
        #ifndef STACK_VECTOR_DISABLE_CHECKS
        if(count == 0 || i > count - 1)
            throw std::out_of_range("Accessing element out of range.");
        #endif

        return d[i]; 
    }

    T pop() 
    { 
        #ifndef STACK_VECTOR_DISABLE_CHECKS
        if(count == 0)
            throw std::out_of_range("Popping vector with size zero.");
        #endif
        
        return d[count--]; 
    }
    void push(const T& v) 
    {
        #ifndef STACK_VECTOR_DISABLE_CHECKS
        if(count == MaxSize)
            throw std::out_of_range("Pushing vector with MaxSize.");
        #endif
        
        d[count++] = v; 
    }

    void unorderedRemove(size_t i) 
    {
        #ifndef STACK_VECTOR_DISABLE_CHECKS
        if(count == 0 || i > count - 1)
            throw std::out_of_range("Removing element out of range.");
        #endif
    
        d[i] = d[--count]; 
    }

    void resize(size_t n)
    {
        #ifndef STACK_VECTOR_DISABLE_CHECKS
        if(n > MaxSize)
            throw std::out_of_range("Resize bigger than MaxSize.");
        #endif

        count = n;
    }

    void clear() { count = 0; }
    size_t size() { return count; }

private:
    T d[MaxSize];
    size_t count = 0;
};

//Part 4 corresponds to step j
void DoPart4()
{
    std::cout << "--- 4 ---" << std::endl;
    auto vec = StackVector<float, 32>(3.2f, 3.3f, 1.3f);
    vec.push(1.4f);
    vec.push(1.6f);
    
    std::cout << "StackVector has " << vec.size() << " elements" << std::endl;
}

// ------------------------------------------------
// -------------------- Part 5 --------------------
// ------------------------------------------------


#define TEST(func) \
{ \
    std::cout << #func << ": "; \
    try \
    { \
        func(); \
        std::cout << "OK" << std::endl; \
    }\
    catch(...) \
    { \
        std::cout << "FAILED" << std::endl; \
    } \
}

#define ASSERT_TRUE(expr) if(!(expr)) throw std::logic_error("FAILED");
#define ASSERT_EXCEPT(except, expr) try { (expr); throw std::logic_error("FAILED");} catch(except e) {}

void TestJobClasses()
{

    auto testProgrammerName = []() {
        auto prog = std::make_unique<Programmer>();
        ASSERT_TRUE(prog->GetName() == "Programmer");
    };

    auto testProgrammerDescription = []() {
        auto prog = std::make_unique<Programmer>();
        ASSERT_TRUE(prog->GetDescription() == "designing and building software.");
    };

    auto testInstanceVector = []() {

        std::vector<std::unique_ptr<Job>> jobs;
        jobs.push_back(std::make_unique<Programmer>());
        jobs.push_back(std::make_unique<Pilot>());
        jobs.push_back(std::make_unique<Programmer>());
        jobs.push_back(std::make_unique<Pilot>());

        ASSERT_TRUE(jobs[0]->GetName() == "Programmer");
        ASSERT_TRUE(jobs[1]->GetName() == "Pilot");
        ASSERT_TRUE(jobs[2]->GetName() == "Programmer");
        ASSERT_TRUE(jobs[3]->GetName() == "Pilot");
    };

    TEST(testProgrammerName);
    TEST(testProgrammerDescription);
    TEST(testInstanceVector);
}

void TestInBoundsFunc()
{
    auto testInBoundsTrue = []() {
        ASSERT_TRUE(IsInBounds<uint32_t>(100, 0, 200));
    };

    auto testInBoundsFalse = []() {
        ASSERT_TRUE(!IsInBounds<int>(-1, 0, 200));
    };

    auto testInBoundsOnLimit = []() {
        ASSERT_TRUE(IsInBounds<uint32_t>(100, 100, 200));
    };

    auto testInBoundsSameValues = []() {
        ASSERT_TRUE(IsInBounds<uint32_t>(0, 0, 0));
    };

    TEST(testInBoundsTrue);
    TEST(testInBoundsFalse);
    TEST(testInBoundsOnLimit);
    TEST(testInBoundsSameValues);
}

void TestContainsTheStringFunc()
{

    auto testContainsTheStringTrue = []() {
        auto s = std::vector<std::string> { "s1", "s2", "s3", "s4", "s4"};

        ASSERT_TRUE(ContainsTheString([](const std::string& t) { return t == "s1"; }, s) == 1);
        ASSERT_TRUE(ContainsTheString([](const std::string& t) { return t == "s2"; }, s) == 1);
        ASSERT_TRUE(ContainsTheString([](const std::string& t) { return t == "s4"; }, s) == 2);
    };

    auto testContainsTheStringFalse = []() {
        auto s = std::vector<std::string> { "s1", "s2", "s3", "s4", "s4"};
        ASSERT_TRUE(ContainsTheString([](const std::string& t) { return t == "s5"; }, s) == 0);
    };

    auto testContainsTheStringEmpty = []() {
        auto s = std::vector<std::string> { };
        ASSERT_TRUE(ContainsTheString([](const std::string& t) { return t == "s1"; }, s) == 0);
    };

    auto testContainsTheStringNullptr = []() {
        auto s = std::vector<std::string> { };
        ASSERT_TRUE(ContainsTheString(nullptr, s) == 0);
    };

    TEST(testContainsTheStringTrue);
    TEST(testContainsTheStringFalse);
    TEST(testContainsTheStringEmpty);
    TEST(testContainsTheStringNullptr);
}

void TestStackVector()
{
    auto testPushStackVector = []() {
        StackVector<float, 4> vec;
        vec.push(1.4f);
        vec.push(2.4f);
        vec.push(3.4f);
        
        ASSERT_TRUE(vec.size() == 3);
    };

    auto testPopStackVector = []() {
        StackVector<float, 4> vec;
        vec.push(1.4f);
        vec.push(2.4f);
        vec.push(3.4f);

        vec.pop();
        vec.pop();

        ASSERT_TRUE(vec.size() == 1);
    };

    auto testCreateStackVectorSmallerSize = []() {
        StackVector<float, 2> vec;
        vec = {1.2f, 1.3f, 1.3f};

        ASSERT_TRUE(vec.size() == 2);
    };

    auto testErrorWhenAddingMoreThanMaxSize = []() {

        ASSERT_EXCEPT(std::out_of_range, (
        { 
            StackVector<float, 2> vec;
            vec.push(1.4f); 
            vec.push(2.4f);
            vec.push(3.4f); 
        }
        ));
    };

    auto testUnorderedRemoveStackVector = []() {

        StackVector<float, 32> vec;
        vec.push(1.4f); 
        vec.push(2.4f);
        vec.push(3.4f);

        vec.unorderedRemove(0);
        ASSERT_TRUE(vec.size() == 2);
        ASSERT_TRUE(vec[0] == 3.4f);
        
        vec.unorderedRemove(1);
        ASSERT_TRUE(vec.size() == 1);
        ASSERT_TRUE(vec[0] == 3.4f);

        vec.unorderedRemove(0);
        ASSERT_TRUE(vec.size() == 0);

        ASSERT_EXCEPT(std::out_of_range, (
        { 
            vec.unorderedRemove(0);
        }
        ));        
    };

    TEST(testPushStackVector);
    TEST(testPopStackVector);
    TEST(testCreateStackVectorSmallerSize);
    TEST(testErrorWhenAddingMoreThanMaxSize);
    TEST(testUnorderedRemoveStackVector);
}

//Part 5 corresponds to step k
void DoPart5() 
{
    std::cout << "--- 5 ---" << std::endl;
    
    TestJobClasses();
    TestInBoundsFunc();
    TestContainsTheStringFunc();
    TestStackVector();
}


// ------------------------------------------------
// --------------------- Main ---------------------
// ------------------------------------------------

int main()
{
    DoPart1();
    DoPart2();
    DoPart3();
    DoPart4();
    DoPart5();

    return 0;
}