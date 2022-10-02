#include <iostream>
#include <string_view>
#include <memory>
#include <vector>
#include <algorithm>
#include <cassert>
#include <variant>
#include <cmath>
#include <sstream>


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Jobs

/// Job interface
class Job {
  public:
    Job() = default;
    virtual ~Job() = default;

    /// Get the name of the job
    [[nodiscard]] virtual auto GetName() const -> std::string_view = 0;

    /// Get the description of the job
    [[nodiscard]] virtual auto GetDescription() const -> std::string_view = 0;

    /// Get the number of hours required by the job
    [[nodiscard]] virtual auto GetNumOfHours() const -> unsigned int = 0;

    /// Describes the work involved on this job
    void DoWork() const {
        std::cout << "My work involves " << GetDescription() << std::endl; 
    }
};

/// Programmer implementation of Job
class Programmer final : public Job {
  public:
    Programmer() = default;
    ~Programmer() = default;

    /// Get the name of the job
    [[nodiscard]] auto GetName() const -> std::string_view final {
        return "Programmer";        
    }

    /// Get the description of the job
    [[nodiscard]] virtual auto GetDescription() const -> std::string_view final {
        return "Programmers write code for computer programs and mobile applications. "
               "They also are involved in maintaining, debugging and troubleshooting systems "
               "and software to ensure that everything is running smoothly.";
    }

    /// Get the number of hours required by the job
    [[nodiscard]] virtual auto GetNumOfHours() const -> unsigned int final {
        return 8 * 21;
    }
};

/// Pilot implementation of Job
class Pilot final : public Job {
  public:
    Pilot() = default;
    ~Pilot() = default;

    /// Get the name of the job
    [[nodiscard]] auto GetName() const -> std::string_view final {
        return "Pilot";        
    }

    /// Get the description of the job
    [[nodiscard]] auto GetDescription() const -> std::string_view final {
        return "A pilot is trained to operate aircraft. As part of their duties, they file flight plans, "
               "perform maintenance checks and ensure the craft is ready for departure. "
               "This includes checking the engine, the navigation equipment and the aircraft's"
               "systems to ensure everything is running properly.";
    }

    /// Get the number of hours required by the job
    [[nodiscard]] auto GetNumOfHours() const -> unsigned int final {
        return 8 * 21;
    }
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Utils

/// Test if the provided values are within a range
template<typename T>
bool IsInBounds(T val, T lower, T higher)
{
    return lower >= val && val <= higher;
}


/// Count number of strings that are validated by predicate / test-function.
template<typename TestFn>
std::size_t ContainsTheString(TestFn&& test_fn, const std::vector<std::string>& strings)
{
    // Using STL:
    // return std::count_if(strings.begin(), strings.end(), test_fn);

    // Alternative:
    std::size_t count = 0;
    for (const auto& str : strings) {
        if (test_fn(str))
            count++;
    }
    return count;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Bonus: Variadic Template show-off..
///
/// Serialization and Deserialization of std::variant
///
/// How? The variant index is serialized and then the variant value.
///
/// Example:
/// std::variant<bool, std::string> variant = "hello";
/// serialized_string == "1\nhello";
/// deserialized_variant == "hello", and type == std::string, and index == 1
///
/// variant = true;
/// serialized_string == "0\n1";
/// deserialized_variant == true, and type == bool, and index == 0
///

namespace detail {

template <size_t I, typename... Ts>
void DeserializeVariantValue(std::istream& is, std::variant<Ts...> &variant)
{
    using Type = std::remove_reference_t<decltype(std::get<I>(variant))>;
    Type value;
    is >> value;
    variant = std::move(value);
}

template <size_t I, typename Variant>
void DeserializeVariantByIndex(std::istream& is, Variant &variant, size_t index)
{
    if (index == I)
        DeserializeVariantValue<I>(is, variant);
}

template <typename Variant, size_t... Is>
void DeserializeVariantExpand(std::istream& is, Variant &variant, size_t index, std::integer_sequence<size_t, Is...>)
{
    (DeserializeVariantByIndex<Is>(is, variant, index), ...);
}

} // namespace detail

/// Serialize std::variant
template <typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::variant<Ts...> &variant)
{
    os << variant.index() << "\n";
    std::visit([&](const auto &val) { os << val; }, variant);
    return os;
}

/// Deserialize std::variant
template <typename... Ts>
std::istream& operator>>(std::istream& is, std::variant<Ts...> &variant)
{
    size_t index = 0;
    char ch;
    is >> index;
    is.get(ch); assert(ch == '\n');
    detail::DeserializeVariantExpand(is, variant, index, std::make_integer_sequence<size_t, sizeof...(Ts)>());
    return is;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Tests

void test_Jobs()
{
    std::vector<std::unique_ptr<Job>> jobs;
    jobs.emplace_back(std::make_unique<Programmer>());
    jobs.emplace_back(std::make_unique<Pilot>());

    for (const auto& job : jobs) {
        std::cout << "Job: " << job->GetName() << '\n'
                  << "Description: " << job->GetDescription() << '\n'
                  << "Number of Hours: " << job->GetNumOfHours() << '\n';
        job->DoWork();
        std::cout << '\n';
    }
}

void test_IsInBounds()
{
    std::cout << "\nTest value within range..\n";
    std::cout << "IsInBounds('g', 'a', 'z'): " << IsInBounds('g', 'a', 'z') << '\n';
    std::cout << "IsInBounds(10UL, 50UL, 100UL): " << IsInBounds(10UL, 50UL, 100UL) << '\n';
    std::cout << "IsInBounds(1.3, 0.1, 9.9): " << IsInBounds(1.3, 0.1, 9.9) << '\n';
}

void test_ContainsTheString()
{
    std::cout << "\nTest ContainsTheString..\n";
    {
        auto strings = std::vector<std::string>{ "one", "two",  "test"};
        auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, strings);
        std::cout << "1° Count: " << count << '\n';
        assert(count == 1);
    }
    {
        auto strings = std::vector<std::string>{ "foo", "bar", "egg", "foo"};
        auto count = ContainsTheString([](const std::string& tested) { return tested == "foo"; }, strings);
        std::cout << "2° Count: " << count << '\n';
        assert(count == 2);
    }
    {
        auto strings = std::vector<std::string>{ "one", "two",  "test"};
        auto count = ContainsTheString([](const std::string& tested) { return tested == "three"; }, strings);
        std::cout << "3° Count: " << count << '\n';
        assert(count == 0);
    }
    {
        auto strings = std::vector<std::string>{};
        auto count = ContainsTheString([](const std::string& tested) { return tested == "apple"; }, strings);
        std::cout << "3° Count: " << count << '\n';
        assert(count == 0);
    }
}

void test_SerializeDeserialize_Variant1()
{
    using Variant = std::variant<char, size_t, std::string>;
    Variant var = 555U;
    assert(var.index() == 1);

    std::cout << "\nTest Variant 1:\n";
    std::stringstream ss;
    ss << var;
    auto serialized = ss.str();
    std::cout << serialized << '\n';
    assert(serialized == "1\n555");

    Variant var_deserialized;
    ss >> var_deserialized;
    assert(var_deserialized.index() == 1);
    assert(std::get<1>(var_deserialized) == 555);
}

void test_SerializeDeserialize_Variant2()
{
    using Variant = std::variant<bool, char, std::string>;
    Variant var = std::string("hello");
    assert(var.index() == 2);

    std::cout << "\nTest Variant 2:\n";
    std::stringstream ss;
    ss << var;
    auto serialized = ss.str();
    std::cout << serialized << '\n';
    assert(serialized == "2\nhello");

    Variant var_deserialized;
    ss >> var_deserialized;
    assert(var_deserialized.index() == 2);
    assert(std::get<2>(var_deserialized) == "hello");
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Main

int main()
{
    test_Jobs();
    test_IsInBounds();
    test_ContainsTheString();
    test_SerializeDeserialize_Variant1();
    test_SerializeDeserialize_Variant2();
    return 0;
}