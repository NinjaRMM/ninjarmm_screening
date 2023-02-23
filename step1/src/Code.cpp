#include <iostream>
#include <vector>
#include <string>
#include <memory>


class Job   //Item "a"
{
    public:     //Item "d"
        virtual std::string GetN() const = 0;
        virtual std::string GetD() const = 0;
        virtual int GetH() const = 0;
        virtual void DoWork() const = 0;
};

class Programmer : public Job   //Item "b"
{
    public:
        std::string GetN() const override {return "Programmer";}
        std::string GetD() const override {return "writing C++ programs";}
        int GetH() const override {return 24;}
        void DoWork() const override {std::cout << "My work involves " << GetD() << "\n";}
};

class Pilot : public Job    //Item "c"
{
    public:
        std::string GetN() const override {return "Pilot";}
        std::string GetD() const override {return "flying jets";}
        int GetH() const override {return 3;}
        void DoWork() const override {std::cout << "My work involves " << GetD() << "\n";}
};

template<typename T>        //item "h"
    bool IsInBounds(T value, T lower, T upper) {return value >= lower && value <= upper;}

// Function to count how many items in a vector match a test function
template<typename T>        //item "i"
    int ContainsTheString(std::function<bool(const T&)> testFunction, const std::vector<T>& vec)
    {
        int count = 0;
        for (const auto& item:vec)
        {
            if (testFunction(item))
            {
            count++;
            }
        }
        return count;
    }

int main()
{
    std::vector<std::unique_ptr<Job>> jobs;
    
    auto programmer = std::make_unique<Programmer>();   //Item "e"
    auto pilot = std::make_unique<Pilot>();
    jobs.push_back(std::move(programmer));      //Item "f"
    jobs.push_back(std::move(pilot));           //Item "f"
    
    for (const auto& job : jobs)        //Item "g"
        {
            std::cout << "Name: " << job->GetN() << "\n";
            std::cout << "Description: " << job->GetD() << "\n";
            std::cout << "Hours required: " << job->GetH() << "\n";
            job->DoWork();
        }
    
    uint32_t httpResponseCode = 503;        //item "h" (test)
    std::cout << "Is http ResponseCode within range? " << IsInBounds<uint32_t>(httpResponseCode, 500, 599) << "\n";
    
    // Test ContainsTheString function
    std::vector<std::string> theStrings = { "one", "two", "test" };     //item "i" (test)
    auto count = ContainsTheString<std::string>([](const std::string& tested) { return tested == "test"; }, theStrings);
    std::cout << "Number of items matching test: " << count << "\n";

    return 0;
}
