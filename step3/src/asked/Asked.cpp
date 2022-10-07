// Application Headers
#include "Asked.hpp"

namespace util {

/// High Order Function to test against a given function
int ContainsTheString(const Hof& func, const VecStr& vec)
{
    int count{0};
    auto lambda = [&](const auto& str) {if (func(str)) {++count;}};

    std::for_each(std::begin(vec), std::end(vec), lambda);
    return count;
}

/// Iterate over a collection of Jobs
void DisplayJobsInfo(const job::VecJob& jobs)
{
    std::for_each(jobs.begin(), jobs.end(), [](const auto& job) {
        std::cout << "Name: "        << job->Name()        << std::endl;
        std::cout << "Description: " << job->Description() << std::endl;
        std::cout << "Hours: "       << job->Hours()       << std::endl;
        job->DoWork();
        std::cout << std::endl;
    });
}

/// Display "***" on the standard output
void PrintSeparator()
{
    std::cout << std::endl;
    std::cout << "*** *** *** *** *** *** *** ***" << std::endl;
    std::cout << std::endl;
}

}// End of namespace util
