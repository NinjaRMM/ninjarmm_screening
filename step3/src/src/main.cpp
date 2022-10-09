#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>
#include <chrono>

#define LOG(...) logDebug(__VA_ARGS__)

constexpr std::string_view PROGRAMMER_NAME = "Programmer";
constexpr std::string_view PROGRAMMER_DESC = "write and test the code that enables software applications to provide functionality.";
constexpr size_t PROGRAMMER_NUM_HOURS = 32;

constexpr std::string_view PILOT_NAME = "Pilot";
constexpr std::string_view PILOT_DESC = "operate aircraft.";
constexpr size_t PILOT_NUM_HOURS = 52;

// Log with Variadic Template
template<typename... Args>
void logDebug(Args... args) {
    (std::cout << ... << args);
    std::cout << std::endl;
}

// A helper function to measure the performance of a function
constexpr std::size_t EXECUTION_TIMES = 100;

class TimerFuncInvoker {
public:
    explicit TimerFuncInvoker(std::size_t times = EXECUTION_TIMES) : times_(times){};

    template<typename Func, typename... Params>
    auto operator()(Func&& func, Params&&... params) {
        const auto& start = std::chrono::high_resolution_clock::now();

        for (auto i = 1; i < times_; ++i) {
            std::forward<Func>(func)(std::forward<Params>(params)...);
        }
        auto resp = std::forward<Func>(func)(std::forward<Params>(params)...);

        const auto& stop = std::chrono::high_resolution_clock::now();
        auto time = (stop - start) / times_;
        return std::make_tuple(resp, time);
    };

private:
    std::size_t times_;
};

class Job {
public:
    virtual ~Job() = default;
    virtual std::string_view name() const = 0;
    virtual std::string_view description() const = 0;
    virtual size_t numHoursRequired() const = 0;
    void doWork() const {
        LOG("My work involves ", description());
    }
};

class Programmer final : public Job {
public:
    std::string_view name() const override {
        return PROGRAMMER_NAME;
    }
    std::string_view description() const override {
        return PROGRAMMER_DESC;
    }
    size_t numHoursRequired() const override {
        return PROGRAMMER_NUM_HOURS;
    }
};

class Pilot final : public Job {
public:
    std::string_view name() const override {
        return PILOT_NAME;
    }
    std::string_view description() const override {
        return PILOT_DESC;
    }
    size_t numHoursRequired() const override {
        return PILOT_NUM_HOURS;
    }
};

template<typename T>
constexpr bool IsInBounds(T value, T min, T max) {
    return value >= min && value <= max;
}

template<typename Functor>
size_t ContainsTheString(Functor&& func, const std::vector<std::string>& theStrings) {
    size_t count = 0;
    for (const auto& value : theStrings) {
        if (func(value)) {
            ++count;
        }
    }
    return count;
}


void testJobClass() {
    LOG("###  testJobClass  ###");
    std::vector<std::unique_ptr<Job>> jobs;
    jobs.push_back(std::make_unique<Programmer>());
    jobs.push_back(std::make_unique<Pilot>());

    for (const auto& job : jobs) {
        LOG("Name:  ", job->name());
        LOG("Number of hours required:  ", job->numHoursRequired());

        job->doWork();
        LOG("");
    }
}

void testIsInBound(uint32_t httpResonseCode) {
    LOG("###  testIsInBound  ###");
    if (IsInBounds<uint32_t>(httpResonseCode, 500, 599)) {
        LOG(httpResonseCode, " is in boundaries between 500 and 599");
    } else {
        LOG(httpResonseCode," is not in boundaries between 500 and 599");
    }
    LOG("");
}

void testContainsTheString() {
    LOG("###  testContainsTheString  ###");
    auto theStrings = std::vector<std::string>{"one", "two", "test"};
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    LOG("Number of matched strings: ", count);
    LOG("");
}

void testTimerFuncInvoker() {
    LOG("###  testTimerFuncInvoker  ###");
    auto theStrings = std::vector<std::string>{"one", "two", "test"};
    auto containsFunc = [](const std::string& tested) { return tested == "test"; };
    auto [resp, time] = TimerFuncInvoker()(ContainsTheString<decltype(containsFunc)>, std::move(containsFunc), theStrings);
    LOG("resp: '", resp, "'; time: ", time.count());
    LOG("");
}

// In order not to pollute the code too much, no automated tests were added. 
// I believe the main idea here would be to show the programming concepts.
int main(int argc, char* argv[]) {
    const std::string errorMessage = "Error: You need to provide a valid value";

    if (argc < 2) {
        std::cout << errorMessage << std::endl;
        return EXIT_FAILURE;
    }

    int httpResonseCode{};
    try {
        httpResonseCode = std::atoi(argv[1]);
        if ((httpResonseCode == 0) && (std::string{argv[1]} != "0")) {
            throw std::runtime_error("");
        }

    } catch (const std::exception&) {
        std::cout << errorMessage << std::endl;
        return EXIT_FAILURE;
    }

    testJobClass();
    testIsInBound(httpResonseCode);
    testContainsTheString();
    testTimerFuncInvoker();

    return EXIT_SUCCESS;
}
