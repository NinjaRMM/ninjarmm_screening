#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <vector>

class Job
{
public:

    Job() = default;
    virtual ~Job() = default;

    virtual std::string GetName() const noexcept = 0;
    virtual std::string GetDescription() const noexcept = 0;
    virtual std::size_t GetRequiredHours() const noexcept = 0;

    void DoWork() const
    {
        std::cout << "My work involves " << GetDescription() << std::endl;
    }
};

class Programmer final : public Job
{
public:

    Programmer() = default;
    ~Programmer() = default;

    std::string GetName() const noexcept override
    {
        return "Programmer";
    }

    std::string GetDescription() const noexcept override
    {
        return "developing softwares";
    }

    std::size_t GetRequiredHours() const noexcept override
    {
        return 8;
    }
};

class Pilot final : public Job
{
public:

    Pilot() = default;
    ~Pilot() = default;

    std::string GetName() const noexcept override
    {
        return "Pilot";
    }

    std::string GetDescription() const noexcept override
    {
        return "flying planes";
    }

    std::size_t GetRequiredHours() const noexcept override
    {
        return 10;
    }
};

template <typename T> auto IsInBounds(const uint32_t value, const T lowerBound, const T upperBound)
{
    return value == std::clamp(value, lowerBound, upperBound);
}

auto LogHttpServerErrorCheck(const uint32_t httpResponseCode)
{
    constexpr uint32_t lowerBound = 500;
    constexpr uint32_t upperBound = 599;
    const auto inBounds = IsInBounds<uint32_t>(httpResponseCode, lowerBound, upperBound);

    std::cout << "HTTP Response Code: " << httpResponseCode << " Lower Bound: " << lowerBound << " Upper Bound: " << upperBound << " Is In Bounds: " << ((inBounds == true) ? "true" : "false") << std::endl;
}


auto ContainsTheString(const std::function<bool(const std::string&)> function, const std::vector<std::string>& theStrings)
{
    return std::count_if(theStrings.cbegin(), theStrings.cend(), function);
}

auto LogTestStringsCount(const std::vector<std::string>& theStrings)
{
    std::cout << "String vector: ";
    std::for_each(theStrings.cbegin(), theStrings.cend(), [](const auto& string) { std::cout << string << ", "; });

    const auto testStringsNumber = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    std::cout << "['test' string count: " << testStringsNumber << "]" << std::endl;
}

template<typename T> auto VariadicMultiplicator(const T arg0)
{
    return arg0;
}

template<typename T, typename... TArgs> auto VariadicMultiplicator(const T arg0, const TArgs... args)
{
    return arg0 * VariadicMultiplicator(args...);
}


int main(int argc, char** argv)
{
    constexpr std::size_t numberOfJobs = 2;
    std::array<std::unique_ptr<Job>, numberOfJobs> jobsCollection{ std::make_unique<Programmer>(), std::make_unique<Pilot>() };

    for (const auto& job : jobsCollection)
    {
        if (job == nullptr) { continue; }

        std::cout << "Job name: " << job->GetName() << std::endl;
        std::cout << "Job description: " << job->GetDescription() << std::endl;
        std::cout << "Job required hours: " << job->GetRequiredHours() << std::endl;
        job->DoWork();
        std::cout << std::endl;
    }

    const std::set<uint32_t> httpResponseCodesSet = { 404, 500, 511, 599, 650 };
    for (const auto httpResponseCode : httpResponseCodesSet)
    {
        LogHttpServerErrorCheck(httpResponseCode);
    }

    auto theStrings = std::vector<std::string>{ "one", "two" };
    for (auto i = 0U; i < 3; ++i)
    {
        LogTestStringsCount(theStrings);
        theStrings.push_back("test");
    }

    std::cout << "Variadic multiplicator result for '42 * 0.5 * 2 * 0.10': " << VariadicMultiplicator(42, 0.5, 2, 0.10) << std::endl;

    return 0;
}