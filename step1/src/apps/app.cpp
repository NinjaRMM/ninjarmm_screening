#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include <step1/job.hpp>
#include <step1/pilot.hpp>
#include <step1/programmer.hpp>
#include <step1/utils.hpp>

void ItemsAtoG(void)
{
    std::cout << "=== Items A to G ===\n";

    auto pilot = std::make_shared<Pilot>("Ayrton Senna");
    auto programmer = std::make_shared<Programmer>("Gabriel Rodrigues");

    std::vector<std::shared_ptr<Job>> jobs;

    jobs.push_back(pilot);
    jobs.push_back(programmer);

    for (auto job : jobs)
    {
        job->SayName();
        job->DescribeJob();
        job->DoWork();
        job->ImproveSkills();
        std::cout << "==========\n";
    }
}

void ItemH(void)
{
    std::cout << "=== Item H ===\n";

    std::vector<uint32_t> values_to_check = {450, 500, 550, 650};

    uint32_t lower_boundary = 500;
    uint32_t upper_boundary = 599;

    for (auto &value_to_check : values_to_check)
    {
        std::cout << "[Inputs] Value to check = " << value_to_check << " | Lower boundary = " << lower_boundary << " | Upper_boundary = " << upper_boundary << "\n";
        std::cout << "[Output] Value IS" << (IsInBounds<uint32_t>(value_to_check, lower_boundary, upper_boundary) ? "" : " NOT") << " within range\n";
        std::cout << "==========\n";
    }

    uint32_t value_to_check = 500;

    std::cout << "[Inputs] Value to check = " << value_to_check << " | Lower boundary = " << lower_boundary << " | Upper_boundary = " << upper_boundary << " | Open range\n";
    std::cout << "[Output] Value IS" << (IsInBounds<uint32_t>(value_to_check, lower_boundary, upper_boundary, RangePolicy::OPEN) ? "" : " NOT") << " within range\n";
    std::cout << "==========\n";

    std::cout << "[Inputs] Value to check = " << value_to_check << " | Lower boundary = " << lower_boundary << " | Upper_boundary = " << upper_boundary << " | Closed range (default)\n";
    std::cout << "[Output] Value IS" << (IsInBounds<uint32_t>(value_to_check, lower_boundary, upper_boundary, RangePolicy::CLOSED) ? "" : " NOT") << " within range\n";
    std::cout << "==========\n";
}

void ItemI(void)
{
    std::cout << "=== Item I ===\n";

    auto the_strings = std::vector<std::string> {"one", "two", "test"};
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, the_strings);

    std::cout << "The strings: ";
    for_each(the_strings.begin(), the_strings.end() - 1, [](const std::string& the_string) { std::cout << the_string << " | "; });
    std::cout << *(the_strings.end() - 1) << "\n";
    std::cout << "Number of occurrences of 'test' in the strings: " << count << "\n";
    std::cout << "==========\n";
}

void ItemJ(void)
{
    std::cout << "=== Item J ===\n";

    auto count = CountIf([](const std::string& tested) { return tested == "test"; }, "one", "two", "test", "test1", "three", "test", "test");

    std::cout << "The strings: one | two | test | test1 | three | test | test\n";
    std::cout << "Number of occurrences of 'test' in the strings: " << count << "\n";
    std::cout << "==========\n";
}

int main()
{
    ItemsAtoG();
    ItemH();
    ItemI();
    ItemJ();

    return 0;
}
