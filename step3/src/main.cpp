#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <memory>
#include <cassert>

class Job {
public:
    Job() = default;
    virtual ~Job() = default;
    virtual std::string Name() const = 0;
    virtual std::string Description() const = 0;
    virtual unsigned long RequiredHours() const = 0;
    void DoWork() const {
        std::ostringstream ss;
        ss << "My work involves " << Description();
        std::cout << ss.str() << std::endl;
    };
};

class Programmer : public Job {
public:
    Programmer() = default;
    ~Programmer() = default;
    std::string Name() const override {
        return "Programmer";
    };
    std::string Description() const override {
        return "converting coffee into code.";
    };
    unsigned long RequiredHours() const override {
        return 42;
    };
};

class Pilot : public Job {
public:
    Pilot() = default;
    ~Pilot() = default;
    std::string Name() const override {
        return "Pilot";
    };
    std::string Description() const override {
        return "operating aircrafts. Blah.";
    };
    unsigned long RequiredHours() const override {
        return 24;
    };
};

template <typename T>
bool IsInBounds(const T checkValue, const T minValue, const T maxValue) {
    return (checkValue >= minValue) && (checkValue <= maxValue);
}

typedef bool (ContainsTheStringFun(const std::string&));
int ContainsTheString(ContainsTheStringFun fun, const std::vector<std::string>& strings) {
    return std::count_if(strings.begin(), strings.end(), fun);
    /* Making it manually (keeping this comment only for the sake of completeness):
    int result = 0;
    for (auto& string : strings) {
        if (fun(string)) {
            result++;
        }
    }
    return result;
    */
}

void fancy_print_rec(int index, std::ostringstream& ss) {
    ss << "-=-=-=-=-=-=-=-" << std::endl;
    std::cout << ss.str();
    return;
}
template <typename T, typename... Ts>
void fancy_print_rec(int index, std::ostringstream& ss, T value, Ts... OtherValues) {
    ss << "-=-=-=-=-=-=-=-" << std::endl <<
        "Element #" << index << ":" << std::endl <<
        value << std::endl;
    fancy_print_rec(index + 1, ss, OtherValues...);
}
template <typename... Ts>
void fancy_print(Ts... Values) {
    std::ostringstream ss;
    fancy_print_rec(1, ss, Values...);
}

/*
    Compiled and executed on Windows (https://godbolt.org/ using compiler "x64 msvc v19.latest).
    Compiled, tested and checked against Valgrind on local Ubuntu Linux:
        ==2292998==
        ==2292998== HEAP SUMMARY:
        ==2292998==     in use at exit: 0 bytes in 0 blocks
        ==2292998==   total heap usage: 35 allocs, 35 frees, 79,941 bytes allocated
        ==2292998==
        ==2292998== All heap blocks were freed -- no leaks are possible
        ==2292998==
        ==2292998== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/
int main() {
    std::unique_ptr<Job> programmer = std::make_unique<Programmer>();
    std::unique_ptr<Job> pilot = std::make_unique<Pilot>();

    std::vector< std::unique_ptr<Job> > jobs;
    jobs.push_back(std::move(programmer));
    jobs.push_back(std::move(pilot));
    for (auto& job : jobs) {
        std::ostringstream ss;
        ss <<
            "Job: " << job->Name() << std::endl <<
            "Description: " << job->Description() << std::endl <<
            "Required Hours: " << job->RequiredHours() << std::endl;
        std::cout << ss.str();
        std::cout << "Doing work... ";
        job->DoWork();
        std::cout << std::endl;
    }

    std::cout << "\n==============\n" << std::endl;
    uint32_t minValue = 500;
    uint32_t maxValue = 599;
    std::array<uint32_t, 5> httpResponseCodes = {404, 500, 550, 599, 600};
    for (auto httpResponseCode : httpResponseCodes) {
        bool isInRange = IsInBounds<uint32_t>(httpResponseCode, 500, 599);
        std::ostringstream ss;
        ss <<
            "Is \"" << httpResponseCode << "\" in range [" <<
            minValue << ", " << maxValue << "]? " <<
            (isInRange ? "Yes" : "No") << std::endl;
        std::cout << ss.str();
    }

    std::cout << "\n==============\n" << std::endl;
    auto theStrings = std::vector<std::string> { "one", "two",  "test"};
    const auto testFun = [](const std::string& tested) { return tested == "test"; };
    const auto nonTestFun = [](const std::string& tested) { return tested != "test"; };
    auto countTest = ContainsTheString(testFun, theStrings);
    auto countNonTest = ContainsTheString(nonTestFun, theStrings);
    std::cout << "Number of \"test\" words: " << countTest << std::endl;
    std::cout << "Number of non-\"test\" words: " << countNonTest << std::endl;

    std::cout << "\n==============\n" << std::endl;
    fancy_print(1, "pilot", "2");

    std::cout << "\n==============\n" << std::endl;
    //Using asserts as I'm not sure which test framework should be used.
    assert(jobs.size() == 2);
    assert(jobs.at(0).get()->Name() == "Programmer");
    assert(jobs.at(0).get()->Description() == "converting coffee into code.");
    assert(jobs.at(0).get()->RequiredHours() == 42);
    assert(jobs.at(1).get()->Name() == "Pilot");
    assert(jobs.at(1).get()->Description() == "operating aircrafts. Blah.");
    assert(jobs.at(1).get()->RequiredHours() == 24);
    assert(IsInBounds<uint32_t>(100, 400, 410) == false);
    assert(IsInBounds<uint32_t>(400, 400, 410) == true);
    assert(IsInBounds<uint32_t>(404, 400, 410) == true);
    assert(IsInBounds<uint32_t>(410, 400, 410) == true);
    assert(IsInBounds<uint32_t>(444, 400, 410) == false);
    assert(IsInBounds<uint8_t>(0, 10, 20) == false);
    assert(IsInBounds<uint8_t>(5, 10, 20) == false);
    assert(IsInBounds<uint8_t>(10, 10, 20) == true);
    assert(IsInBounds<uint8_t>(15, 10, 20) == true);
    assert(IsInBounds<uint8_t>(20, 10, 20) == true);
    assert(IsInBounds<uint8_t>(25, 10, 20) == false);
    assert(IsInBounds<uint8_t>(255, 10, 20) == false);
    const auto filterApples = [](const std::string& tested) { return tested == "apple"; };
    const auto filterBananas = [](const std::string& tested) { return tested == "banana"; };
    const auto filterCherries = [](const std::string& tested) { return tested == "cherry"; };
    const auto fruits1 = std::vector<std::string> {
        std::string("apple"), std::string("banana"), std::string("cherry"), std::string("apple")
    };
    const auto fruits2 = std::vector<std::string> {
        std::string("cherry"), std::string("banana"), std::string("banana"), std::string("cherry")
    };
    assert(ContainsTheString(filterApples, fruits1) == 2);
    assert(ContainsTheString(filterBananas, fruits1) == 1);
    assert(ContainsTheString(filterCherries, fruits1) == 1);
    assert(ContainsTheString(filterApples, fruits2) == 0);
    assert(ContainsTheString(filterBananas, fruits2) == 2);
    assert(ContainsTheString(filterCherries, fruits2) == 2);
    std::cout << "All tests passed" << std::endl;
    return 0;
}
