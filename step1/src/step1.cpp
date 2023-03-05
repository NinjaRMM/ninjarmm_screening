#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
// 	a. Create a class Job
class Job
{

    /*	d. The job class should provide an interface that can do the following
        i. Get the name of the job
        ii. Get the description of the job.
        iii. Get the number of hours required by the job
        iv. A DoWork function that prints out “My work involves ” + the description.
    */
private:
    std::string m_Name;
    std::string m_Description;
    int m_RequiredHours;

protected:
    Job(const std::string &Name, const std::string Description, int RequiredHours) : m_Name(Name),
                                                                                     m_Description(Description),
                                                                                     m_RequiredHours(RequiredHours)
    {
    }

public:
    virtual const std::string &GetName() const
    {
        return m_Name;
    }
    virtual const std::string &GetDescription() const
    {
        return m_Description;
    }
    virtual const int GetRequiredHours() const
    {
        return m_RequiredHours;
    }
    virtual void DoWork() const
    {
        std::cout << "My Work involves " << m_Description;
    }
};

//	b. Create a subclass Programmer
class Programmer : public Job
{
public:
    Programmer() : Job("Programmer", "Writting amazing programs", 8)
    {
    }
};
// 	c. Create a subclass Pilot
class Pilot : public Job
{
public:
    Pilot() : Job("Pilot", "Flying planes", 6)
    {
    }
};

// 	h. Create a template function that will test if the provided values are within a range. Call the function and print out the inputs and result. The following should execute assuming httpResonse is a uint32_t value:
template <typename T>
bool IsInBounds(T input, T lowerRange, T upperRange)
{
    return (input >= lowerRange) && (input <= upperRange);
}

//	i. Create a function that will take a vector of strings and a test function, It should return how many items in the collection matched the test. Then print out the result.

int countIf(std::function<bool(const std::string &)> test, const std::vector<std::string> &input)
{
    return std::count_if(input.begin(), input.end(), test);
}

// 	j. Write any other code you feel would show off your understanding of C/C++ and programming in general. Bonus: Variadic Template /w usage.
template <typename R, typename... Types1, typename... Types2>
void runTest(const std::string &TestName, R (*func)(Types1...), R expected, Types2 &&...input)
{

    if (expected == func(input...))
    {
        std::cout << "TEST " << TestName << " PASSED \n";
    }
    else
    {
        std::cout << "TEST " << TestName << " FAILED \n";
    }
}

int main(int argc, char const *argv[])
{
    // e.Dynamically allocate each type of job.
    std::shared_ptr<Programmer>
        programmer = std::make_shared<Programmer>();
    std::shared_ptr<Pilot> pilot = std::make_shared<Pilot>();

    //	f. Store each instance in a collection.
    std::vector<std::shared_ptr<Job>> jobs = {std::move(programmer), std::move(pilot)};

    std::cout << "---------- CLASSES ----------\n";

    //	g. Iterate the collection and call each of the available interface functions.
    for (const std::shared_ptr<Job> job : jobs)
    {
        if (!job)
        {
            continue;
        }
        std::cout << "Job Name: " << job->GetName() << "\n";
        std::cout << "Job Description: " << job->GetDescription() << "\n";
        std::cout << "Job required hours: " << job->GetRequiredHours() << "\n";
        job->DoWork();
        std::cout << "\n";
    }

    std::cout << "---------- TEMPLATE FUNCTION ----------\n";
    const uint32_t httpResponseCode = 501;
    const uint32_t lowerLimit = 500;
    const uint32_t upperLimit = 599;
    bool result = IsInBounds<uint32_t>(httpResponseCode, lowerLimit, upperLimit);
    std::cout << "The Input: " << httpResponseCode << " is" << (result ? "" : " not") << " in the bounds: " << lowerLimit << "-" << upperLimit << "\n";

    std::cout << "---------- COUNT IF ----------\n";

    const auto theStrings = std::vector<std::string>{"one", "two", "test"};
    int count = countIf([](const std::string &tested)
                        { return tested == "test"; },
                        theStrings);
    std::cout << count << " strings in the vector matched the test \n";

    // k. Please add passing unit tests.
    runTest(
        "countIfTest",
        countIf, 1, [](const std::string &tested)
        { return tested == "test"; },
        theStrings);

    runTest(
        "countIfa",
        countIf, 2, [](const std::string &tested)
        { return tested == "a"; },
        std::vector<std::string>({"a", "a", "b", "c"}));

    runTest(
        "countIfStringLength",
        countIf, 2, [](const std::string &tested)
        { return tested.length() > 5; },
        std::vector<std::string>({"a", "a", "b", "c", "", "abcdfg", "123456"}));

    runTest(
        "inBounds501",
        IsInBounds<int>, true, 501, 500, 599);

    runTest(
        "inBounds400",
        IsInBounds<int>, false, 400, 500, 599);

    runTest(
        "inBounds0",
        IsInBounds<int>, false, 0, 500, 599);

    return 0;
}
