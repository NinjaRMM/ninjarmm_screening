#include <functional>
#include <iostream>
#include <memory>
#include <vector>

class Job
{
  public:
    Job(){};
    virtual ~Job(){};

    virtual std::string GetName() const
    {
        return "Job";
    }

    virtual std::string GetDescription() const
    {
        return "doing stuff";
    }

    virtual unsigned int GetRequiredHours() const
    {
        return 40;
    }

    void DoWork() const
    {
        std::cout << "My work involves " << GetDescription() << std::endl;
    }
};

class Programmer : public Job
{
  public:
    Programmer()
    {
    }
    ~Programmer()
    {
    }

    std::string GetName() const override
    {
        return "Programmer";
    }

    std::string GetDescription() const override
    {
        return "making stuff";
    }

    unsigned int GetRequiredHours() const override
    {
        // Bootcamp!
        return 16;
    }
};

class Pilot : public Job
{
  public:
    Pilot()
    {
    }
    ~Pilot()
    {
    }

    std::string GetName() const override
    {
        return "Pilot";
    }

    std::string GetDescription() const override
    {
        return "moving stuff in the air";
    }

    unsigned int GetRequiredHours() const override
    {
        // Airline pilot!
        return 1500;
    }
};

template <class T> bool IsInBounds(T value, T min, T max)
{
    return value >= min && value <= max;
}

unsigned int stringsMatchesTest(const std::function<bool(std::string)> test, const std::vector<std::string> strings)
{
    unsigned int count = 0;
    for (const auto &str : strings)
    {
        if (test(str))
        {
            count++;
        }
    }
    return count;
}

int main(int, char **)
{
    std::cout << "Step 2, First exercise, requirements a to g: " << std::endl;
    std::vector<std::unique_ptr<Job>> jobs;
    jobs.push_back(std::make_unique<Programmer>());
    jobs.push_back(std::make_unique<Pilot>());
    for (const auto &job : jobs)
    {
        std::cout << "Name: " << job->GetName() << std::endl;
        std::cout << "Description: " << job->GetDescription() << std::endl;
        std::cout << "Hours required: " << job->GetRequiredHours() << std::endl;
        job->DoWork();
        std::cout << std::endl;
    }

    std::cout << "Step 2, Second exercise, requirement h: " << std::endl;
    unsigned int httpResonseCode = 404;
    unsigned int minResponseCode = 500;
    unsigned int maxResponseCode = 599;
    std::cout << "Value: " << httpResonseCode << ", min: " << minResponseCode << ", max: " << maxResponseCode
              << std::endl;
    std::cout << "Is in Bounds: " << IsInBounds<uint32_t>(httpResonseCode, 500, 599) << std::endl;

    std::cout << "Step 2, Third exercise, requirement i: " << std::endl;
    auto theStrings = std::vector<std::string>{"one", "two", "test"};
    auto count = stringsMatchesTest([](const std::string &tested) { return tested == "test"; }, theStrings);
    if (count == 0)
    {
        std::cout << "None have matched the test";
    }
    else if (count == 1)
    {
        std::cout << count << " has matched the test";
    }
    else
    {
        std::cout << count << " have matched the test";
    }
}
