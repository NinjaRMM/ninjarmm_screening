#include <Step1.hpp>

int main()
{
    // Dynamically allocate each type of job
    std::vector<std::unique_ptr<Job>> jobs;
    jobs.push_back(std::make_unique<Programmer>());
    jobs.push_back(std::make_unique<Pilot>());

    // Iterate the collection and call each of the available interface functions
    for (const auto& job : jobs)
    {
        std::cout << "Name: " << job->get_name() << std::endl;
        std::cout << "Description: " << job->get_description() << std::endl;
        std::cout << "Hours required: " << job->get_hours_required() << std::endl;
        job->DoWork();
    }

    // Test if the provided value (503) is in the range of HTTP response codes (500-599)
    constexpr uint32_t http_response_code = 503;
    auto is_response_code_in_bounds = is_in_bounds<uint32_t>(http_response_code, 500, 599);
    std::cout << "HTTP response code " << http_response_code << " is ";
    std::cout << (is_response_code_in_bounds ? "in" : "not in") << " range" << std::endl;

    // Test number of "test" in a collection of strings
    std::vector<std::string> the_strings{"one", "two", "test"};
    int count = count_matches(the_strings, [](const std::string& tested) { return tested == "test"; });
    std::cout << count << " string(s) contain the word 'test'" << std::endl;

    // Variadic lambda using auto
    auto print = [](auto... args) { ((std::cout << args << " "), ...); std::cout << std::endl;};
    print("Hello", "world", 1, "I am", 7.5, "years", "old");

    // Variadic lambda, old school
    print_args(7.5, "+", 7.5, "=", (7.5 + 7.5));
    return 0;
}