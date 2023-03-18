#include <src.hpp>
#include <iostream>

int main()
{

    try
    {
        auto programmer = factory<Programmer>("Programmer", "Keyboard warrior", 40);
        auto pilot = factory<Pilot>("Pilot", "Guy who doesn't like the ground enough", 30);
        static_assert(std::is_base_of<Job, Programmer>::value, "Programmer is not a Job");
        static_assert(std::is_base_of<Job, Pilot>::value, "Pilot is not a Job");

        std::vector<std::unique_ptr<Job>> jobs;
        jobs.reserve(2);

        jobs.emplace_back(std::move(programmer));
        jobs.emplace_back(std::move(pilot));

        for( auto&& j : jobs) {

            std::cout << j->name() << "\n";
            std::cout << j->description() << "\n";
            std::cout << j->hoursRequired() << "\n";
            std::cout << j->doWork() << "\n";
        }

        {
            const auto theStrings = std::vector<std::string> { "one", "two",  "test"};
            const auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
            assert(count == 1);
        }

        {
            const auto theStrings = std::vector<std::string> { "one", "two",  "one"};
            const auto count = ContainsTheString([](const std::string& tested) { return tested == "one"; }, theStrings);
            assert(count == 2);
        }

        {
            const auto httpResonseCode = HttpResponse::SERVER_ERROR;
            assert(IsInBounds<uint32_t>(httpResonseCode, 500, 599) == true);
        }

        {
            const auto httpResonseCode = HttpResponse::SERVER_ERROR;
            assert(IsInBounds<uint32_t>(httpResonseCode, 499, 599) == false);
        }

        {
            const auto httpResonseCode = HttpResponse::SERVER_ERROR;
            assert(IsInBounds<uint32_t>(httpResonseCode, 500, 600) == false);
        }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }
    catch(const char* c) {
        std::cerr << c << '\n';
        return -1;
    }
return 0;
}