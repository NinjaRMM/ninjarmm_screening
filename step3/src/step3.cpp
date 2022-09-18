#include <cstdio>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <cinttypes>
#include <iostream>
#include <set>
#include <deque>
#include <list>
#include <forward_list>

#if __cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
    #include <string_view>
#endif

// Section A: Create a class Job
//--------------------------------------
class Job {
    public:
        Job() = default;
#if __cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
        Job(std::string_view name, std::string_view description, size_t numHoursRequired)
            : mName{ name }
            , mDescription{ description }
            , mNumHoursRequired{ numHoursRequired } { }
#else
        Job(std::string name, std::string description, size_t numHoursRequired)
            : mName{ std::move(name) }
            , mDescription{ std::move(description) }
            , mNumHoursRequired{ numHoursRequired } { }
#endif
        virtual ~Job() = default;   // ensure it will be called due to virtual

        // Section D: The job class should provide an interface that can do the following
        // Note: These methods are not virtual because I don't think the getters are going to change

        // D i. Get the name of the job
        const std::string & GetName() const             { return mName; }
        // D ii. Get the description of the job
        const std::string & GetDescription() const      { return mDescription; }
        // D iii. Get the number of hours required by the job
        size_t              GetNumHoursRequired() const { return mNumHoursRequired; }

        // D iv. A DoWork function that prints out "My work involves " + the description.
        // Note: This method is not const because at this point we do not know what the classes that inherit from us will do
        virtual void        DoWork() {
            printf("My work involves %s\n", mDescription.c_str()); // I prefer printf over cout
        }

    protected:
        std::string mName;
        std::string mDescription;
        size_t      mNumHoursRequired {};
};

// Section B: Create a subclass Programmer
//--------------------------------------
class Programmer : public Job {
    public:
        Programmer() : Job("Programmer", "do cool stuff with computers", 123) {}

        // Just in case you were expecting override a virtual function
        void DoWork() override {
            Job::DoWork();
        }
};

// Section C: Create a subclass Pilot
//--------------------------------------
class Pilot : public Job {
    public:
        Pilot() : Job("Pilot", "travel around the world", 456) {}
};

// Section H: Create a template function that will test if the provided values are within a range.
//--------------------------------------
template <typename T>
bool
IsInBounds(T value, T min, T max) {
    return value >= min && value <= max;
}

// Test H: Call the function and print out the inputs and result.
// The following should execute assuming httpResonse is a uint32_t value:
// IsInBounds<uint32_t>(httpResonseCode, 500, 599)
//
// MSVC cannot deduce template argument for std::function. It works with gcc
//--------------------------------------
//template <typename T1, typename T2, typename T3>
//typename std::enable_if<std::is_convertible<T2, T1>::value && std::is_convertible<T3, T1>::value, void>::type
//ShowResult(std::function<bool(T1, T1, T1)> func, T1 value, T2 min, T3 max) {
//    auto result = func(value, T1(min), T1(max));
//    std::cout << std::boolalpha;
//    std::cout << "Is value " << value << " >= " << min << " <= " << max << " = " << result << "\n";
//}

//--------------------------------------
template <typename Func, typename T1, typename T2, typename T3>
typename std::enable_if<std::is_convertible<T2, T1>::value && std::is_convertible<T3, T1>::value, void>::type
ShowResult(Func func, T1 value, T2 min, T3 max) {
    auto result = func(value, T1(min), T1(max));
    std::cout << std::boolalpha;
    std::cout << "Is value " << value << " >= " << min << " <= " << max << " = " << result << "\n";
}

// Section I: Create a function that will take a vector of strings and a test function,
// It should return how many items in the collection matched the test.
//--------------------------------------
size_t
ContainsTheString(std::function<bool(const std::string &)> func, const std::vector<std::string> &strings) {
    size_t count = 0;

    for(const auto &str : strings) {
        count += func(str) == true ? 1 : 0;
    }

    return count;
}

// Section J: Write any other code you feel would show off your understanding of C/C++
// and programming in general. Bonus: Variadic Template /w usage.

// TODO: Think something else

// Call a function with several values  [C++17]
//--------------------------------------
template<typename Func, typename T, typename... Args>
void
ForEach(Func func, T value, Args... args) {
    func(value);
    if constexpr (sizeof...(Args) > 0)
        ForEach(func, args...);
}

// Check if all elements are in bounds [C++17]
// Note: We have to split the function, like ForEach, if C++17 is not available
//--------------------------------------
template <typename T1, typename T2, typename... Args>
bool
AllInBounds(T1 min, T1 max, T2 value, Args... args) {
    if constexpr (sizeof...(Args) > 0)
        return (value >= T2(min)) && (value <= T2(max)) && AllInBounds(min, max, args...);
    else
        return (value >= T2(min)) && (value <= T2(max));
}

// Check if any element is in bounds [C++17]
// Note: Split the function like ForEach if C++17 is not available
//--------------------------------------
template <typename T1, typename T2, typename... Args>
bool
AnyInBounds(T1 min, T1 max, T2 value, Args... args) {
    if constexpr (sizeof...(Args) > 0)
        return (value >= T2(min)) && (value <= T2(max)) ? true : AnyInBounds(min, max, args...);
    else
        return (value >= T2(min)) && (value <= T2(max));
}

// Weird thing to demonstrate C++ knowledge. Add all elements of a container
//--------------------------------------
template<template <typename...> class C, typename... Args>
auto
AddContainer(C<Args...> container) {
    typename C<Args...>::value_type result{};

    for (const auto &v : container)
        result += v;

    return result;
}

//--------------------------------------
int
main(int argc, char *argv[]) {
    // avoid warnings (we could also remove argc and argv or use [[maybe_unused]] since C++17)
    (void) argc;
    (void) argv;

    // Check memory leaks
    #if defined(_MSVC) && !defined(NDEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    #endif

    // Section E: Dynamically allocate each type of job.
    // I use shared pointers because I prefer not to delete the objects myself.
    // As I don't have more context, I have preferred shared_ptr over unique_ptr.
    auto programmer = std::make_shared<Programmer>();
    auto pilot      = std::make_shared<Pilot>();

    // Section F: Store each instance in a collection.
    std::vector<std::shared_ptr<Job>>  jobs;

    jobs.emplace_back(programmer);
    jobs.emplace_back(pilot);

    // Section G: Iterate the collection and call each of the available interface functions.
    for(const auto &job : jobs) {
        printf("Job name: %s\n", job->GetName().c_str());
        printf("Job description: %s\n", job->GetDescription().c_str());
        printf("Job time: %zu\n", job->GetNumHoursRequired());
        job->DoWork();
        printf("--\n");
    }

    // Test H b: Call the function and print out the inputs and result.
    uint32_t httpResonseCode = 501;
    bool result = IsInBounds<uint32_t>(httpResonseCode, 500, 599);
    std::cout << std::boolalpha;
    std::cout << "Is value " << httpResonseCode << " >= " << 500 << " <= " << 599 << " = " << result << "\n";

    // Some more tests
    const uint32_t min = 500, max = 599;

    ShowResult(&IsInBounds<uint32_t>, min-1,           min, max);
    ShowResult(&IsInBounds<uint32_t>, min,             min, max);
    ShowResult(&IsInBounds<uint32_t>, (min + max) / 2, min, max);
    ShowResult(&IsInBounds<uint32_t>, max,             min, max);
    ShowResult(&IsInBounds<uint32_t>, max+1,           min, max);

    ShowResult(&IsInBounds<uint32_t>, 23,              min, max);
    ShowResult(&IsInBounds<double>, 23.5, 23.0f, 24.0);
    ShowResult(&IsInBounds<float>, 22.99f, 23.0f, 24.0);

    // Test I: Create a function that will take a vector of strings and a test function,
    // It should return how many items in the collection matched the test.
    // Then print out the result.
    auto theStrings = std::vector<std::string> { "one", "two", "test"};
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);

    printf("\n--\nContainsTheString: %d\n", int(count));

    // Test J
    printf("--\n");
    ForEach([](auto v) { std::cout << v << "\n";  }, 1, 2.5, 'a', "hola!");

    printf("--\n");
    bool all = AllInBounds(min, max, min, min + 1, max - 1, max, min * 1.1f, min * 0.5);
    std::cout << "All values are in bounds: " << all << "\n";

    bool any = AnyInBounds(min, max, min-1, max+1, 1234, 1.0f, min);
    std::cout << "Any values are in bounds: " << any << "\n";

    printf("--\n");

    std::set<int>           s{ 1,2,3 };
    std::deque<int>         d{ 1,2,3 };
    std::list<int>          l{ 1,2,3 };
    std::forward_list<int>  fl{ 1,2,3 };
    std::vector<int>        v{ 1,2,3 };

    auto result_s  = AddContainer(s);
    auto result_d  = AddContainer(d);
    auto result_l  = AddContainer(l);
    auto result_fl = AddContainer(fl);
    auto result_v  = AddContainer(v);

    printf("%d, %d, %d, %d, %d (must be all equal)\n", result_s, result_d, result_l, result_fl, result_v);

    return 0;
}
