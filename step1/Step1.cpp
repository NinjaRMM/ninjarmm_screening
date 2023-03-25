#include <Step1.hpp>
#include <unordered_map>
#include <variant>

template<class> inline constexpr bool always_false_v = false;

void operate_jobs();
void check_http_response_code();
void check_number_of_test_in_collection();
void variadic_lambda_with_auto();
void variadic_lambda_old_school();
void variant_usage();

int main()
{
    operate_jobs();
    check_http_response_code();
    check_number_of_test_in_collection();
    variadic_lambda_with_auto();
    variadic_lambda_old_school();
    variant_usage();
        
    return 0;
}

void operate_jobs()
{
    std::vector<std::unique_ptr<Job>> jobs;
    jobs.push_back(std::make_unique<Programmer>());
    jobs.push_back(std::make_unique<Pilot>());

    for (const auto& job : jobs)
    {
        std::cout << "Name: " << job->get_name() << std::endl;
        std::cout << "Description: " << job->get_description() << std::endl;
        std::cout << "Hours required: " << job->get_hours_required() << std::endl;
        job->DoWork();
    }
}

void check_http_response_code()
{
    constexpr uint32_t http_response_code = 503;
    auto is_response_code_in_bounds = is_in_bounds<uint32_t>(http_response_code, 500, 599);
    std::cout << "HTTP response code " << http_response_code << " is ";
    std::cout << (is_response_code_in_bounds ? "in" : "not in") << " range" << std::endl;
}

void check_number_of_test_in_collection()
{
    std::vector<std::string> the_strings{"one", "two", "test"};
    int count = count_matches(the_strings, [](const std::string& tested) { return tested == "test"; });
    std::cout << count << " string(s) contain the word 'test'" << std::endl;
}

void variadic_lambda_with_auto()
{
    auto print = [](auto... args) { ((std::cout << args << " "), ...); std::cout << std::endl;};
    print("Hello", "world", 1, "I am", 7.5, "years", "old");
}

void variadic_lambda_old_school()
{
    print_args(7.5, "+", 7.5, "=", (7.5 + 7.5));
}

void variant_usage()
{
    std::unordered_map<std::string, std::variant<Programmer, Pilot>> name_job_map;
    name_job_map["Ion"] = Programmer{};
    name_job_map["Mikel"] = Pilot{};

    for (const auto& [name, job] : name_job_map)
    {
        std::cout << name << " is a ";
        std::visit([](const auto& job) { std::cout << job.get_name(); }, job);
        std::cout << std::endl;
    }

    for (const auto& [name, job] : name_job_map)
    {
        std::cout << name << " what are you doing? ";
        std::visit([](auto&& job){
                using T = std::decay_t<decltype(job)>;
                if constexpr (std::is_same_v<T, Programmer>)
                {
                    job.program();
                }
                else if constexpr (std::is_same_v<T, Pilot>)
                {
                    job.ride_a_plane();
                }
                else
                {
                    static_assert(always_false_v<T>, "non-exhaustive visitor!");
                }
        } , job);
    }
}