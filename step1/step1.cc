#include <cassert>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

using namespace std::string_literals;

// Implementation details of the Fmt utility
namespace detail {

// Fmt() needs ToString for any arguments. Provide some basics
const std::string&
ToString(bool v)
{
    static const std::string T = "true"s;
    static const std::string F = "false"s;
    return v ? T : F;
}

const std::string&
ToString(const std::string& v)
{
    return v;
}

// Use ostream::operator<<() as a last resort
template <typename T>
std::string
ToString(const T& v)
{
    std::ostringstream os;
    os << v;
    return os.str();
}

// Terminal implementation of fmt, when there's no more args
void
fmt_(std::string& out, std::string_view fmt)
{
    out.append(fmt);
}

// Recursive format, find the next format marker in fmt, replace it with the next
// argument, and repeat until we run out of either format markers or arguments.
template <typename T, typename... U>
void
fmt_(std::string& out, std::string_view fmt, const T& arg, const U&... args)
{
    // Enable ADL for ToString, use the ones in detail:: as a last resort.
    using detail::ToString;
    auto pos = fmt.find("{}");
    if (pos != std::string_view::npos) {
        if (pos != 0) {
            out.append(fmt, 0, pos);
        }
        out.append(ToString(arg));
        fmt_(out, fmt.substr(pos + 2), args...);
    } else {
        out.append(fmt);
    }
}

} // namespace detail

// Perform a format operation on the input string fmt, replacing each instance
// of the marker "{}" with the next argument. "{}" could be extended to do
// positional formatting in the future by using tokens like "{0}", "{1}", etc...
// Arguments can be either strings, anything that supports ostream operator<<,
// or have a free method std::string ToString(T) defined.
//
// This is somewhat naive and does not detect argument count mismatches. Extra
// arguments will be ignored, extra markers without a corresponding
// argument will appear in the output directly.
//
// Future enhancements:
// - Support positional arguments ("{0}", "{1}")
// - Ignore escaped braces ("\\{")
template <class... Args>
std::string
Fmt(const std::string& fmt, Args... args)
{
    std::string out;
    detail::fmt_(out, std::string_view(fmt), args...);
    return out;
}

// Small wrapper to curry a precision as a format argument for a float:
//   float x = 2.33;
//   std::cout << Fmt("Value: {}", WithPrecision(x, 1));
// will print "Value: 2.3"
// enable_if to check T is a float.
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
struct NumberWithPrecision {
    NumberWithPrecision(T v, int p) : val(v), precision(p) {}
    T val;
    int precision;
};

// Provide a ToString hook for Fmt to find via ADL
template <typename T>
std::string
ToString(const NumberWithPrecision<T>& n)
{
    std::ostringstream os;
    os << std::fixed << std::setprecision(n.precision) << n.val;
    return os.str();
}

// Helper to deduce the right instantiation of NumberWithPrecision
template <typename T>
NumberWithPrecision<T>
WithPrecision(T val, int precision = 1)
{
    return NumberWithPrecision<T>{val, precision};
}

// Join a sequence of strings together with a given separator.
// Naive version of boost::algorithm::join().
template <typename Iterator>
std::string
Join(Iterator first, Iterator last, const std::string& sep)
{
    std::string out;
    // Append the first element
    if (first != last) {
        out.append(*first);
        ++first;
    }
    // Every remaining element gets prefixed with the separator
    for (; first != last; ++first) {
        out.append(sep);
        out.append(*first);
    }
    return out;
}

// Join with a range instead of iterators
template <typename Range>
std::string
Join(const Range& input, const std::string& sep)
{
    return Join(std::begin(input), std::end(input), sep);
}


class Job {
public:
    Job() {}
    virtual ~Job() = default;

    // Everyone drinks coffee. It affects some more than others.
    void DrinkCoffee(uint8_t cups);
    // Get how much caffeine they have. As time elapses (from work or rest)
    // the caffeine level decreases to zero.
    float Caffeine() const { return caffeine_; }
    // Perform some work. This is non-virtual to handle things that
    // always happen, subclasses hook Work_() to inject their functionality
    // as needed.
    // std::chrono::duration might be a better option here.
    void Work(float hours);
    // Rest. Some jobs need more than others.
    void Rest(float hours);

    // Return how much longer they can work without needing something
    virtual float MaxWork() const = 0;

    // Ask the worker for a favorite quote.
    virtual std::string Quote() const = 0;

    // Ask the worker what they've accomplished.
    virtual std::string Status() const = 0;

protected:
    static void CheckTime(float hours);

    // Hook for the subclass to perform any specific work handling
    // Default does nothing.
    virtual void Work_(float hours) {}
    // Hook for the subclass to perform any specific rest handling.
    // Default does nothing.
    virtual void Rest_(float hours) {}

    // Elapse time (consume caffeine)
    void ElapseTime(float hours);

private:
    static constexpr float HOURS_PER_CUP{2.5};
    float caffeine_{0.0f};
};

void
Job::DrinkCoffee(uint8_t cups)
{
    caffeine_ += (cups * HOURS_PER_CUP);
}

void
Job::Work(float hours)
{
    CheckTime(hours);
    auto max = MaxWork();
    if (hours > max) {
        throw std::domain_error(
            Fmt("Requested {} hours exceeds maximum {}", hours, max));
    }
    Work_(hours);
    ElapseTime(hours);
}

void
Job::Rest(float hours)
{
    CheckTime(hours);
    Rest_(hours);
    ElapseTime(hours);
}

void
Job::CheckTime(float hours)
{
    if (hours <= 0) {
        throw std::domain_error("Hours must be positive");
    }
}

void
Job::ElapseTime(float hours)
{
    CheckTime(hours);
    // Consume caffeine linerarly until we run out.
    // Which isn't really correct, but I need something simple for
    // demonstration purposes.
    caffeine_ = std::max(caffeine_ - hours, 0.0f);
}

// Job representing a programmer.
// Programmers can work forever, as long as you keep them caffeinated,
// and they produce code.
class Programmer : public Job {
public:
    std::string Quote() const override;
    float MaxWork() const override;
    std::string Status() const override;

protected:
    void Work_(float hours) override;

private:
    int linesOfCode_{0};
};

std::string
Programmer::Quote() const
{
    return "The pool on the roof must have a leak. --Dade Murphy"s;
}

float
Programmer::MaxWork() const
{
    // We burn 1 unit of Caffeine an hour (see above) so that's how
    // long we can work.
    return Caffeine();
}

std::string
Programmer::Status() const
{
    return Fmt("Wrote {} lines of code.", linesOfCode_);
}

void
Programmer::Work_(float hours)
{
    int lines = (int)(hours * 5);
    linesOfCode_ += lines;
}

// Pilots fly aircraft and produce logged hours. Pilots have a duty
// cycle, they can fly MAX_DUTY hours in one shift and then must rest
// for MIN_REST hours before going back on duty. They drink coffee like
// everyone else but it has no visible effect.
class Pilot : public Job {
public:
    std::string Quote() const override;
    float MaxWork() const override;
    std::string Status() const override;

protected:
    void Work_(float hours) override;
    void Rest_(float hours) override;

private:
    static constexpr float MAX_DUTY{10.0};
    static constexpr float MIN_REST{12.0};

    // How many hours in the current duty cycle. A zero value means they are
    // resting and the first call to Work_() will start counting again.
    float worked_{0.0};
    // How many hours in the current rest cycle.
    float rested_{0.0};
    // How many hours flown total.
    float totalHours_{0.0};
};

float
Pilot::MaxWork() const
{
    if (worked_) {
        // On-shift, so how much time is left?
        return MAX_DUTY - worked_;
    } else if (rested_ >= MIN_REST) {
        // Rest period completed
        return MAX_DUTY;
    }
    // Need to rest more
    return 0;
}

std::string
Pilot::Status() const
{
    return Fmt("Logged {} hours", WithPrecision(totalHours_, 1));
}

void
Pilot::Work_(float hours)
{
    rested_ = 0;
    worked_ += hours;
    assert(worked_ <= MAX_DUTY);
    totalHours_ += hours;
}

void
Pilot::Rest_(float hours)
{
    worked_ = 0;
    rested_ += hours;
}

std::string
Pilot::Quote() const
{
    return "I fly. I pilot. --Russell Casse"s;
}

// Template function to determine if a value is within the range [lb, ub].
// T must provide operator<().
template <typename T>
bool
IsInBounds(const T val, const T lb, const T ub)
{
    // Logically lb <= val <= ub, but only needs operator<
    return !(val < lb || ub < val);
}

// The STL (and boost) canonically does func(range, predicate); I'm
// putting the arguments in this order to match the instructions. Personally
// I find predicate last easier to read with multi-line lambdas.
template <class Predicate>
unsigned
ContainsTheString(Predicate p, const std::vector<std::string>& input)
{
    unsigned rv = 0;
    for (const auto& s : input) {
        if (p(s)) {
            ++rv;
        }
    }
    return rv;
}

// Tokenize a comma separated line.
std::vector<std::string>
SplitCSV(const std::string& line)
{
    std::vector<std::string> toks;
    std::string::size_type last = 0;
    for (;;) {
        // Find the next comma
        auto f = line.find(',', last);
        if (std::string::npos == f) {
            // End of the line
            toks.push_back(line.substr(last));
            break;
        }
        // Push back the token up to the mark we just found
        toks.push_back(line.substr(last, f - last));
        last = f + 1;
    }
    return toks;
}

template <typename T>
using Vec2d = std::vector<std::vector<T>>;

// Parse a CSV stream into a 2d vector (row first) of strings
// For the purposes of this excercise I'm assuming the input file
// is relatively small and the overhead of tokenizing and copy for
// transpose is acceptable. For a significantly larger dataset I'd
// look at tokenizing into string_view<>s instead, but then that
// also has it's own complexity in managing the lifecycle of the
// underlying strings.
Vec2d<std::string>
ParseCSV(std::istream& in)
{
    std::string line;
    Vec2d<std::string> res;
    for (;;) {
        std::getline(in, line);
        // If needed, we could differentiate betwen EOF and other non-good
        // conditions here and throw an IO exception; for the purposes of
        // demonstrating this algorithm I'm skipping it
        if (!in.good()) {
            break;
        }
        res.push_back(SplitCSV(line));
    }
    return res;
}

// Error thrown when we hit a row with a different number of columns than
// the rest when transposing a 2d vector.
class TransposeDimensionError : public std::domain_error {
public:
    TransposeDimensionError(int row, int acol, int ecol)
      : std::domain_error(
            Fmt("Row {} has {} columns, required {}", row, acol, ecol))
    {
    }
};

// Transpose / Rotate a 2d vector
template <typename T>
Vec2d<T>
Transpose(const Vec2d<T>& in)
{
    Vec2d<T> res;
    if (in.empty()) {
        return res;
    }
    res.resize(in[0].size());
    // For each row, take the columns and append them to rows sequentially
    // in the output. The algorithm needs to know the indicies, so the
    // usual collection iterator alone isn't sufficient.
    for (auto i = 0; i < in.size(); ++i) {
        if (in[0].size() != in[i].size()) {
            throw TransposeDimensionError(i, in[i].size(), in[0].size());
        }
        for (auto j = 0; j < in[i].size(); ++j) {
            res[j].push_back(in[i][j]);
        }
    }
    return res;
}

// Really simple UT framework for demonstration purposes rather than pulling in CxxTest
namespace Test {

// Individual failure marker within a test case. I don't really use these much
// past the point of logging them at create time now, but they could be used for
// more analysis.
struct Failure {
    std::string file;
    int line;
    std::string fname;
    std::string what;
};

// Forward-declare Suite so we can friend it.
class Suite;

// Represent a test case, i.e. a single function with one or more T_ASSERT* calls in it.
using CaseFunc = std::pair<std::string, void (*)()>;
class Case {
public:
    // This constructor should be considered private, it's exposed so
    // emplace_back works. Use the initializer list constructor on Suite along
    // with the T_CASE macro to stringify the function name properly.
    Case(CaseFunc func) : name_(func.first), f_(func.second) {}
    Case(Case&&) = default;
    Case& operator=(Case&&) = default;

    // Record an assertion failure
    void MarkFailure(const std::string& file,
                     int line,
                     const std::string& func,
                     const std::string& what);

    const std::string& Name() const { return name_; }
    const std::vector<Failure>& Failures() const { return failures_; }
    bool Ok() const { return failures_.empty(); }

    // Get a pointer to the currently running case, so the assertion macros can
    // reach back and call MarkFailure without having to pass it around. There's
    // no thread-safety here, a process could have multiple threads to support a
    // test in progress, but only one test suite/case is running at a time in
    // the entire process.
    static Case& Current() { return *current_; }

private:
    friend class Test::Suite;

    Case(const Case&) = delete;
    Case& operator=(const Case&) = delete;

    bool Run();

    std::string name_;
    void (*f_)();
    std::vector<Failure> failures_;

    static Case* current_;
};

Case* Case::current_{nullptr};

void
Case::MarkFailure(const std::string& file,
                  int line,
                  const std::string& func,
                  const std::string& what)
{
    std::cerr << Fmt("FAIL: {} at {} ({}:{})\n", what, func, file, line);
    failures_.push_back(Failure{file, line, func, what});
}

bool
Case::Run()
{
    std::cerr << Fmt("=== {} Start\n", name_);
    assert(!current_);
    failures_.clear();
    current_ = this;

    // f() shouldn't throw, but if it does, that's a failure in and of itself
    try {
        f_();
    } catch (std::exception& e) {
        MarkFailure(__FILE__, __LINE__, name_, e.what());
    } catch (...) {
        MarkFailure(__FILE__, __LINE__, name_, "Unspecified exception");
    }

    assert(this == current_);
    current_ = nullptr;
    std::cerr << Fmt("=== {} {}\n", name_, Ok() ? "Passed"s : "FAILED"s);
    return Ok();
}

class Suite {
public:
    Suite(std::initializer_list<CaseFunc> args)
    {
        cases_.reserve(args.size());
        for (const auto& f : args) {
            cases_.emplace_back(f);
        }
    }

    bool Run();

    // How many passed/failed. Only valid after a call to Run().
    unsigned CountPassed() const { return countPassed_; }
    unsigned CountFailed() const { return countFailed_; }

private:
    std::vector<Case> cases_;
    unsigned countPassed_{0};
    unsigned countFailed_{0};
};

bool
Suite::Run()
{
    countPassed_ = 0;
    countFailed_ = 0;
    int passed = 0;
    for (auto& c : cases_) {
        if (c.Run()) {
            ++countPassed_;
        } else {
            ++countFailed_;
        }
    }

    std::cerr << Fmt("Results {} / {} Passed\n", countPassed_, cases_.size());
    if (countFailed_) {
        std::cerr << "Failed: \n";
        for (const auto& c : cases_) {
            if (!c.Ok()) {
                std::cerr << "  " << c.Name() << "\n";
            }
        }
        return false;
    }
    return true;
}

} // namespace Test

using TestSuite = Test::Suite;

// Handle to a test case function of {name, function pointer}
#define T_CASE(f) Test::CaseFunc(#f, f)

#define TX_FAIL(file__, line__, method__, msg)                                 \
    do {                                                                       \
        Test::Case::Current().MarkFailure(file__, line__, method__, msg);      \
    } while (false)

#define T_FAIL(msg) TX_FAIL(__FILE__, __LINE__, __func__, msg)

// Basic test assertion when p is false
#define T_ASSERT(p)                                                            \
    if (!(p)) {                                                                \
        TX_FAIL(__FILE__, __LINE__, __func__, Fmt("Assertion \"{}\"", #p));    \
    }

// Assert a == b
#define T_ASSERT_EQUAL(a, b)                                                   \
    {                                                                          \
        const auto& a__ = (a);                                                 \
        const auto& b__ = (b);                                                 \
        if (!(a__ == b__)) {                                                   \
            TX_FAIL(                                                           \
                __FILE__,                                                      \
                __LINE__,                                                      \
                __func__,                                                      \
                Fmt("Assertion \"{} == {}\" ({} == {})", #a, #b, a__, b__));   \
        }                                                                      \
    }

// Assert a != b
#define T_ASSERT_NOT_EQUAL(a, b)                                               \
    {                                                                          \
        const auto& a__ = (a);                                                 \
        const auto& b__ = (b);                                                 \
        if (!(a__ != b__)) {                                                   \
            TX_FAIL(                                                           \
                __FILE__,                                                      \
                __LINE__,                                                      \
                __func__,                                                      \
                Fmt("Assertion \"{} != {}\" ({} != {})", #a, #b, a__, b__));   \
        }                                                                      \
    }
// Simple tests to verify assertion failures.
void
test_Fail()
{
    T_FAIL("Expected failure");
}

void
test_Assert()
{
    const char* msg = "Hello World";
    T_ASSERT(!msg);
}

void
test_AssertEqual()
{
    int a = 3;
    T_ASSERT_EQUAL(4, a);
    T_ASSERT_NOT_EQUAL(3, a);
}

// Begin actual test cases
void
test_Job()
{
    // Demonstrate the lifecycle of the Job class hierarchy
    std::vector<std::unique_ptr<Job>> jobs;
    jobs.push_back(std::make_unique<Pilot>());
    jobs.push_back(std::make_unique<Programmer>());

    int i = 0;

    auto flags = std::cout.flags();
    std::cout.precision(1);
    std::cout << std::fixed;
    for (auto& e : jobs) {
        std::cout << "jobs[" << i << "]\n";
        auto avail = e->MaxWork();
        // Everybody starts unable to work
        T_ASSERT_EQUAL(0, (int)avail);
        std::cout << "  Max Work: " << avail << "\n";
        std::cout << "  (Rest)\n";

        // sleep and drink some coffee
        e->Rest(12.0);
        e->DrinkCoffee(2.0);
        std::cout << "  Caffeine level: " << e->Caffeine() << "\n";

        avail = e->MaxWork();

        // Should be able to work now
        T_ASSERT_NOT_EQUAL(0, (int)avail);
        std::cout << "  Max Work: " << avail << "\n";
        std::cout << "  (Work " << avail / 2 << ")\n";
        e->Work(avail / 2);

        avail = e->MaxWork();
        T_ASSERT_NOT_EQUAL(0, (int)avail);
        std::cout << "  Max Work: " << avail << "\n";
        std::cout << "  Caffeine level: " << e->Caffeine() << "\n";
        std::cout << "  (Work " << avail << ")\n";
        e->Work(avail);

        avail = e->MaxWork();
        std::cout << "  Max Work: " << avail << "\n";
        // Should have maxed out, whatever it was
        T_ASSERT_EQUAL(0, (int)avail);
        std::cout << "  (Coffee)\n";
        e->DrinkCoffee(2.0);
        avail = e->MaxWork();
        std::cout << "  Max Work: " << avail << "\n";
        if (avail > 0) {
            std::cout << "  [Looks like a programmer]\n";
        } else {
            std::cout << "  [Coffee didn't fix it, not a Programmer]\n";
            std::cout << "  (Rest)\n";
            e->Rest(16);
            avail = e->MaxWork();
        }
        std::cout << "  Max Work: " << avail << "\n";
        T_ASSERT_NOT_EQUAL(0, (int)avail);
        std::cout << "  (Work " << avail << ")\n";
        e->Work(avail);
        std::cout << "  Status: " << e->Status() << "\n";
        std::cout << "  Quote: " << e->Quote() << "\n";

        ++i;
    }
    std::cout.flags(flags);
}

void
test_IsInBounds()
{
    struct TestVal {
        uint32_t val;
        uint32_t min;
        uint32_t max;
        bool expected;
    };

    const auto inputs = std::vector<TestVal>{
        TestVal{500, 500, 599, true },
        TestVal{510, 500, 599, true },
        TestVal{599, 500, 599, true },
        TestVal{499, 500, 599, false},
        TestVal{600, 500, 599, false},
        TestVal{1,   10,  100, false},
        TestVal{10,  0,   9,   false},
    };

    for (const auto& e : inputs) {
        auto r = IsInBounds<uint32_t>(e.val, e.min, e.max);
        std::cout << Fmt("IsInBounds({}, {}, {}): {} (expected: {})\n",
                         e.val,
                         e.min,
                         e.max,
                         r,
                         e.expected);
        T_ASSERT_EQUAL(e.expected, r);
    }
}

void
test_ContainsTheString()
{
    auto theStrings = std::vector<std::string>{"one", "two", "three"};
    auto count = ContainsTheString(
        [](const std::string& tested) { return tested == "test"; },
        theStrings);
    T_ASSERT_EQUAL(count, 0);

    count = ContainsTheString(
        [](const std::string& tested) { return tested.size() == 3; },
        theStrings);
    T_ASSERT_EQUAL(2, count);
}

void
test_ParseCSV()
{
    // Parse and transpose the CSV file
    auto stream = std::ifstream("test.csv");
    auto parsed = ParseCSV(stream);
    auto transposed = Transpose(parsed);

    // Stringify the transposed version
    std::ostringstream out;
    for (const auto& col : transposed) {
        out << Join(col, " ") << "\n";
    }
    auto str = out.str();

    // Dump it to the console
    std::cout << str;

    // Validate it against what we expected
    auto expected = "id 0 1 2\ntype bird cat dog\nname blue felix toto\n"s;
    T_ASSERT(str == expected);
}

void
test_TransposeMismatch()
{
    // Test the assertion when we transpose a non-uniform matrix
    Vec2d<int> input{
        std::vector<int>{1, 2, 3},
        std::vector<int>{3, 4},
        std::vector<int>{5, 6, 7},
    };

    try {
        auto out = Transpose(input);
        T_FAIL("Should have thrown TransposeDimensionError");
    } catch (TransposeDimensionError& e) {
        T_ASSERT("Row 1 has 2 columns, required 3"s == e.what());
    }
}

void
test_Fmt()
{
    auto res = Fmt("Hello {}", "World"s);
    std::cout << res << "\n";
    T_ASSERT_EQUAL("Hello World"s, res);

    res = Fmt("{}{}", "Hello", " World");
    std::cout << res << "\n";
    T_ASSERT_EQUAL("Hello World"s, res);

    res = Fmt("An integer: {}", 3);
    std::cout << res << "\n";
    T_ASSERT_EQUAL("An integer: 3"s, res);

    float pi = 3.14159;
    res = Fmt("Pi: {}", WithPrecision(pi, 1));
    std::cout << res << "\n";
    T_ASSERT_EQUAL("Pi: 3.1", res);

    res = Fmt("Pi: {}", WithPrecision(pi, 3));
    std::cout << res << "\n";
    T_ASSERT_EQUAL("Pi: 3.142"s, res);

    // Unhandled right now
    // Excess marker
    res = Fmt("Hello {}");
    std::cout << res << "\n";
    T_ASSERT_EQUAL("Hello {}"s, res);

    // Excess argument
    res = Fmt("{} {}", 1, 2, 3);
    std::cout << res << "\n";
    T_ASSERT_EQUAL("1 2", res);
}

void test_Join()
{
    std::vector<std::string> vals{};
    auto res = Join(vals, ", ");
    std::cout << res << "\n";
    T_ASSERT_EQUAL(""s, res);

    vals.push_back("a");
    res = Join(vals, ", ");
    std::cout << res << "\n";
    T_ASSERT_EQUAL("a"s, res);

    vals.push_back("b");
    vals.push_back("c");
    res = Join(vals, ", ");
    std::cout << res << "\n";
    T_ASSERT_EQUAL("a, b, c"s, res);
}

extern "C" {
int
main(int argc, char** argv)
{
    std::cerr << "UT self-test (failure expected):\n";
    TestSuite failSuite{T_CASE(test_Fail),
                        T_CASE(test_Assert),
                        T_CASE(test_AssertEqual)};
    (void)failSuite.Run();
    // Everything should have failed
    if (failSuite.CountFailed() != 3 || failSuite.CountPassed() != 0) {
        std::cerr << "UT self-test failed.\n";
        return 1;
    }
    std::cerr << "\n";

    // initialize a test suite and execute it
    // Were I to productize this framework, this would be auto-generated,
    // but why would I reinvent CxxTest?
    std::cerr << "Primary tests:\n";
    TestSuite suite{
        T_CASE(test_Job),
        T_CASE(test_IsInBounds),
        T_CASE(test_ContainsTheString),
        T_CASE(test_ParseCSV),
        T_CASE(test_TransposeMismatch),
        T_CASE(test_Fmt),
        T_CASE(test_Join),
    };

    auto passed = suite.Run();
    return passed ? 0 : 1;
}
} // extern "C"
