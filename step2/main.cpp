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

int main(int, char **)
{
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
}
