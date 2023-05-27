#include "step1/Pilot.hpp"
#include "step1/Programmer.hpp"

#include <cstdlib>
#include <iostream>
#include <functional>
#include <memory>
#include <ranges>
#include <vector>

inline void PrintSeparator() {
    std::cout << std::endl << std::string(80, '-') << std::endl << std::endl;
}

template<class T>
bool IsInBounds(T value, T lowerBound, T higherBound) {
    return !std::greater<T>()(lowerBound, value)
        && !std::greater<T>()(value, higherBound);
}

std::size_t ContainsTheString(std::function<bool(const std::string&)> tester,
                              const std::vector<std::string>& strings) {
    using std::ranges::distance, std::views::filter;

    auto filteredString = strings | filter(tester);
    return distance(filteredString);
}

int main(int argc, char** argv) {
    using Category = step1::Pilot::Category;
    using step1::Programmer;
    using step1::Pilot;
    using step1::Job;
    using PilotInitList = std::initializer_list<Category>;
    using ProgrammerInitList = std::initializer_list<std::string>;

    auto pilot = std::make_unique<Pilot>(
        8, PilotInitList{Category::Formula1, Category::Formula2});
    auto programmer =
        std::make_unique<Programmer>(ProgrammerInitList{"C++"});

    std::vector<std::unique_ptr<Job<>>> jobs;

    jobs.emplace_back(std::move(pilot));
    jobs.emplace_back(std::move(programmer));

    jobs.emplace_back(std::make_unique<Pilot>(
        12, PilotInitList{Category::Formula1, step1::Pilot::Category::Formula2,
                          step1::Pilot::Category::WEC}));

    jobs.emplace_back(
        std::make_unique<Programmer>(ProgrammerInitList{"C++", "Rust"}));

    for (const auto& job: jobs) {
        std::cout << "Job Name: " << job->GetName() << std::endl;
        std::cout << "Job Description: " << job->GetDescription() << std::endl;
        std::cout << "Job Hours: " << job->GetRequiredHours() << std::endl;
        std::cout << std::endl << "Now Working..." << std::endl;
        job->DoWork();

        PrintSeparator();
    }

    uint32_t httpResonseCode = 503;
    uint32_t lower = 500, higher = 599;

    std::cout << "Checking if " << httpResonseCode << " is Between " << lower
              << " and " << higher << ": "
              << (IsInBounds<uint32_t>(httpResonseCode, lower, higher)
                      ? "Yes, it is!"
                      : "No, it is not!")
              << std::endl;

    PrintSeparator();

    auto theStrings = std::vector<std::string>{"one", "two", "test"};
    auto count = ContainsTheString(
        [](const std::string &tested) { return tested == "test"; },
        theStrings);
    std::cout << count << " of " << theStrings.size()
        << " strings passed the ContainsTheString test" << std::endl;
    return EXIT_SUCCESS;
}
