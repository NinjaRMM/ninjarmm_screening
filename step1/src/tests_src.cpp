#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "src.hpp"
#include "doctest.h"

TEST_SUITE("src") {
    TEST_CASE("Testing Jobs interface: step1 (a - g)") {
        auto job = factory<Job>("Contractor", "Spray Foam", 65);
        auto programmer = factory<Programmer>("Programmer", "Keyboard warrior", 40);
        auto pilot = factory<Pilot>("Pilot", "Guy who doesn't like the ground enough", 30);

        static_assert(std::is_base_of<Job, Programmer>::value, "Programmer is not a Job");
        static_assert(std::is_base_of<Job, Pilot>::value, "Pilot is not a Job");

        std::vector<std::unique_ptr<Job>> jobs;
        jobs.reserve(3);

        jobs.emplace_back(std::move(job));
        jobs.emplace_back(std::move(programmer));
        jobs.emplace_back(std::move(pilot));
        CHECK(jobs.size() == 3);

        CHECK(jobs[0]->name() == "Contractor");
        CHECK(jobs[0]->description() == "Spray Foam");
        CHECK(jobs[0]->hoursRequired() == 65);
        CHECK(jobs[0]->doWork() == "My work involves Spray Foam");

        CHECK(jobs[1]->name() == "Programmer");
        CHECK(jobs[1]->description() == "Keyboard warrior");
        CHECK(jobs[1]->hoursRequired() == 40);
        CHECK(jobs[1]->doWork() == "My work involves Keyboard warrior");

        CHECK(jobs[2]->name() == "Pilot");
        CHECK(jobs[2]->description() == "Guy who doesn't like the ground enough");
        CHECK(jobs[2]->hoursRequired() == 30);
        CHECK(jobs[2]->doWork() == "My work involves Guy who doesn't like the ground enough");

        for( auto&& j : jobs) {

            std::cout << j->name() << "\n";
            std::cout << j->description() << "\n";
            std::cout << j->hoursRequired() << "\n";
            std::cout << j->doWork() << "\n";
        }
    }

    TEST_CASE("testing IsInBounds(): step1 (h)") {
        // Focused on server errors to match the example in Requirements.txt
        SUBCASE("testing HTTP response code within 500s") {
            constexpr auto httpResonseCode = HttpResponse::SERVER_ERROR;
            CHECK(IsInBounds<uint32_t>(httpResonseCode, 500, 599) == true);
        }

        SUBCASE("testing HTTP response code outside of 500s"){
            constexpr auto httpResonseCode = HttpResponse::SERVER_ERROR;
            CHECK(IsInBounds<uint32_t>(httpResonseCode, 499, 599) == false);
        }

        SUBCASE("testing HTTP response code outside of 500s"){
            constexpr auto httpResonseCode = HttpResponse::SERVER_ERROR;
            CHECK(IsInBounds<uint32_t>(httpResonseCode, 500, 600) == false);
        }
    }

    TEST_CASE("testing ContainsTheString(): step1 (i)") {
        SUBCASE("No matching strings"){
            const auto theStrings = std::vector<std::string> { "one", "two",  "three"};
            const auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
            CHECK(count == 0);
        }

        SUBCASE("One matching string"){
            const auto theStrings = std::vector<std::string> { "one", "two",  "test"};
            const auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
            CHECK(count == 1);
        }

       SUBCASE("Two matching strings")
        {
            const auto theStrings = std::vector<std::string> { "one", "two",  "one"};
            const auto count = ContainsTheString([](const std::string& tested) { return tested == "one"; }, theStrings);
            CHECK(count == 2);
        }
    }
}