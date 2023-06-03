#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>

/**
 * Job class
 */
class Job
{
    public:

        virtual std::string getJobName() const = 0;
        virtual std::string getJobDescription() const = 0;
        virtual float getJobHours() const = 0;

        void DoWork() const
        {
            std::cout << "My work involves " << getJobDescription() << std::endl;
        }
};

/**
 * Programmer class that inherits from Job class
 */
class Programmer: public Job
{
    public:

        std::string getJobName() const override
        {
            return "Programmer";
        }

        std::string getJobDescription() const override
        {
            return "creating an awesome application";
        }

        float getJobHours() const override
        {
            return 30.0f;
        }
};

/**
 * Pilot class that inherits from Job class
 */
class Pilot: public Job
{
    public:

        std::string getJobName() const override
        {
            return "Pilot";
        }

        std::string getJobDescription() const override
        {
            return "flying from Madrid to London";
        }

        float getJobHours() const override
        {
            return 2.5f;
        }
};
/**
 * Implementation of IsInBounds function assuming inclusive ranges.
 * Additionally, the range check will be done between the last arguments independant of order.
 */
template<typename T>
bool IsInBounds(const T &value, const T &low, const T &high)
{
    return std::min(low, high) <= value && value <= std::max(low, high);
}

/**
 * Implementation of ContainsTheString.
 */
template<typename Functor>
unsigned long ContainsTheString(const Functor &lambda, const std::vector<std::string> &strings)
{
    return std::count_if(strings.begin(), strings.end(), lambda);
}

/**
 * Implementation of Variadic Print when called with one argument.
 */
template<typename T>
void variadicPrint(const T &value)
{
    std::cout << value << " ";
}

/**
 * Implementation of Variadic Print when called with more than one argument.
 */
template<typename T, typename ... Pending>
void variadicPrint(const T &value, const Pending& ... pendingElements)
{
    std::cout << value << " ";
    variadicPrint(pendingElements...);
    std::cout << std::endl;
}

/**
 * Implementation of Variadic Sum when called with one argument.
 */
template<typename T>
T variadicSum(const T &value)
{
    return value;
}

/**
 * Implementation of Variadic Sum when called with more than one argument.
 */
template<typename T, typename ... Pending>
auto variadicSum(const T &value, const Pending& ... pendingElements)
{
    return value + variadicSum(pendingElements...);
}


// TESTING
TEST_CASE("Test Job class related code: Step1 objectives 1.a to 1.g", "[Objectives 1.a to 1.g]")
{
    std::vector<std::unique_ptr<Job>> jobs;

    jobs.push_back(std::make_unique<Programmer>());
    jobs.push_back(std::make_unique<Pilot>());

    for (const auto &job : jobs)
    {
        if (dynamic_cast<Programmer*>(job.get()))
        {
            REQUIRE(job->getJobName() == "Programmer");
            REQUIRE(job->getJobDescription() == "creating an awesome application");
            REQUIRE(job->getJobHours() == 30.0f);
        }
        else if (dynamic_cast<Pilot*>(job.get()))
        {
            REQUIRE(job->getJobName() == "Pilot");
            REQUIRE(job->getJobDescription() == "flying from Madrid to London");
            REQUIRE(job->getJobHours() == 2.5f);
        }
        job->DoWork();
    }

    jobs.clear();
}

TEST_CASE("Test template function for values are in a range: Step1 objective 1.h", "[Objective 1.h]")
{
    int values[] = {4, 5, 10};
    bool isInBounds = IsInBounds<int>(values[0], values[1], values[2]);
    std::cout << "The result of calling IsInBounds with values ";
    std::copy(std::begin(values), std::end(values), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "is " << (isInBounds ? "inside bounds." : "outside bounds.") << std::endl;
    REQUIRE(isInBounds == false);

    REQUIRE(IsInBounds<int>(11, 11, 12) == true);
    REQUIRE(IsInBounds<int>(10, 0, 20) == true);
    REQUIRE(IsInBounds<int>(23, 20, 23) == true);
    REQUIRE(IsInBounds<int>(12, 5, 11) == false);
    REQUIRE(IsInBounds<int>(10, 15, 5) == true);

    REQUIRE(IsInBounds<uint32_t>(10, 5, 20) == true);
    REQUIRE(IsInBounds<uint32_t>(21, 5, 20) == false);

    REQUIRE(IsInBounds<int16_t>(10, 5, 20) == true);
    REQUIRE(IsInBounds<int16_t>(21, 5, 20) == false);

    REQUIRE(IsInBounds<float>(10, 5, 20) == true);
    REQUIRE(IsInBounds<float>(21, 5, 20) == false);

    REQUIRE(IsInBounds<double>(10, 5, 20) == true);
    REQUIRE(IsInBounds<double>(21, 5, 20) == false);

    REQUIRE(IsInBounds<char>('b', 'a', 'c') == true);
    REQUIRE(IsInBounds<char>('d', 'a', 'c') == false);
}

TEST_CASE("Test function that will take a vector of strings and a test function", "[Objective 1.i]" )
{
    auto theStrings = std::vector<std::string> { "one", "two", "test" };
    auto lambda = [](const std::string &tested){return tested == "test";};
    auto count = ContainsTheString(lambda, theStrings);
    std::cout << "The result of the call to ContainsTheString is " << count << std::endl;

    REQUIRE(count == 1);

    REQUIRE(ContainsTheString([](const std::string &tested){return tested == "three";}, theStrings) == 0);

    theStrings.push_back("test");
    REQUIRE(ContainsTheString(lambda, theStrings) == 2);
}

TEST_CASE("Test Variadic Template with usage: Step1 objective 1.j", "[Objective 1.j]" )
{
    REQUIRE(variadicSum(1, 2, 3, 4) == 10);

    variadicPrint(1.1, 2, 'd', 5.5f);

    REQUIRE(variadicSum(1.1, 2, 'd', 5.5f) == 108.6);
}
