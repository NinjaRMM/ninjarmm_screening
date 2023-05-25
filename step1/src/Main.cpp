#include "step1/Pilot.hpp"
#include "step1/Programmer.hpp"

#include <cstdlib>
#include <iostream>
#include <functional>
#include <memory>
#include <ranges>
#include <vector>

inline void PrintDivision() {
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
    auto pilot = std::make_unique<step1::Pilot>();
    auto programmer = std::make_unique<step1::Programmer>();

    std::vector<std::unique_ptr<step1::Job<>>> jobs;

    jobs.emplace_back(std::move(pilot));
    jobs.emplace_back(std::move(programmer));

    for (const auto& job: jobs) {
        std::cout << "Job Name: " << job->GetName() << std::endl;
        std::cout << "Job Description: " << job->GetDescription() << std::endl;
        std::cout << "Job Hours: " << job->GetRequiredHours() << std::endl;
        std::cout << std::endl << "Now Working..." << std::endl;
        job->DoWork();

        PrintDivision();
    }

    uint32_t httpResonseCode = 503;
    uint32_t lower = 500, higher = 599;

    std::cout << "Checking if " << httpResonseCode << " is Between " << lower
              << " and " << higher << ": "
              << (IsInBounds<uint32_t>(httpResonseCode, lower, higher)
                      ? "Yes, it is!"
                      : "No, it is not!")
              << std::endl;

    PrintDivision();

    auto theStrings = std::vector<std::string>{"one", "two", "test"};
    auto count = ContainsTheString(
        [](const std::string &tested) { return tested == "test"; },
        theStrings);
    std::cout << count << " of " << theStrings.size()
        << " strings passed the ContainsTheString test" << std::endl;
    return EXIT_SUCCESS;
}
