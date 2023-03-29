// Copyright (c) 2023. Alexandre Horst
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
//  associated documentation files (the "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
//  of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
//   conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
// OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef NINJARMM_SCREENING_STEP1_H
#define NINJARMM_SCREENING_STEP1_H

#include <iostream>
#include <utility>
#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>

#ifdef HAS_BOOST
#include <boost/program_options.hpp>
#endif

#include <shared_mutex>

using namespace std;
using namespace std::chrono;
#ifdef HAS_BOOST
namespace po = boost::program_options;
#endif

/// \brief A C++ class called "Job" with private member variables, and public const member functions.
///
/// This is a C++ class called "Job" that has three private member variables: "name_", "description_", and "hours_". It
/// has four public member functions:
///
/// 1. "name()" is a const function that returns a const reference to the "name_" member variable.
/// 2. "description()" is a const function that returns a const reference to the "description_" member variable.
/// 3. "hours()" is a const function that returns the "hours_" member variable.
/// 4. "DoWork()" is a const function that prints out a message to cout about the job's description.
///
/// The constructor takes three arguments, a string for the name, a string for the description, and an unsigned 32-bit
/// integer for the number of hours. The const keyword is used to ensure that member functions do not modify the state
/// of the object they belong to. The nodiscard attribute is used to indicate that the return values of the functions
/// should not be ignored.
class Job {
private:
    string name_;
    string description_;
    uint32_t hours_;
public:
    /// This method returns the name of the job.
    [[nodiscard]] inline const string &name() const {
        return name_;
    }

    /// This method returns the description of the job.
    [[nodiscard]] inline const string &description() const {
        return description_;
    }

    /// This method returns the number of hours required by the job.
    [[nodiscard]] uint32_t hours() const {
        return hours_;
    }

    /// This method prints out a message about the job's description.
    void DoWork() const {
        cout << "My work involves " << description_ << endl;
    }

    /// This is the constructor for the Job class. It takes in a string for the name, a string for the description, and
    /// an unsigned 32-bit integer for the number of hours. It then initializes the member variables with the passed
    /// arguments.
    Job(string n, string d, uint32_t h) : name_(std::move(n)), description_(std::move(d)), hours_(h) {}
};

/// \brief A C++ class called "Programmer" that inherits from the "Job" class.
///
/// This code defines a `Programmer` class that inherits from a base class `Job`. The `Programmer` class has a
/// constructor that takes in a `name` (of type `string`), a `description` (also of type `string`), and an `hours`
/// variable (of type `uint32_t`). This constructor then calls the constructor of the base class `Job` and passes the
/// `name`, `description`, and `hours` arguments to it using the `std::move()` function.
class Programmer : public Job {
public:
    /// This is the constructor for the Programmer class.
    Programmer(string name, string description, uint32_t hours) : Job(std::move(name), std::move(description), hours) {}
};

/// \brief A C++ class called "Pilot" that inherits from the "Job" class.
///
/// This code defines a `Pilot` class that inherits from a base class `Job`. The `Pilot` class has a constructor that
/// takes in a `name` (of type `string`), a `description` (also of type `string`), and an `hours` variable
/// (of type `uint32_t`). This constructor then calls the constructor of the base class `Job` and passes the `name`,
/// `description`, and `hours` arguments to it using the `std::move()` function.
class Pilot : public Job {
public:
    /// This is the constructor for the Pilot class.
    Pilot(string name, string description, uint32_t hours) : Job(std::move(name), std::move(description), hours) {}
};

/// \brief A C++ class called EntryPoint that takes arguments and return an exit code.
///
/// This code defines a C++ class named "EntryPoint" which takes in the number of arguments and an array of character
/// pointers as parameters in its constructor. The class has a protected variable "argc" to hold the number of arguments
/// and another protected variable "argv" to hold the array of character pointers. The class also has a private variable
/// "exit_code_" initialized to 0 and a public method "setExitCode" to set its value.
///
/// The code also defines an explicit conversion operator that allows an object of the EntryPoint class to be implicitly
/// converted to an integer. The returned integer is the value of the private variable "exit_code_".
class EntryPoint {
public:
    /**
     * @brief A constructor for the EntryPoint class.
     *
     * Initializes private member variables with passed arguments.
     *
     * @param p_argc An integer representing the number of command line arguments.
     * @param p_argv A pointer to a constant character array representing the command line arguments.
     */
    explicit EntryPoint(int p_argc, const char **p_argv) : argc(p_argc), argv(p_argv) {};

    /**
     * @brief An explicit conversion operator that allows an object of the EntryPoint class to be implicitly
     * converted to an integer.
     *
     * @return int The value of the private variable "exit_code_".
     */
    explicit operator int() const { return exit_code_; };
protected:
    int argc; ///< The number of command line arguments.
    const char **argv; ///< The command line arguments.
private:
    int exit_code_{0}; ///< The exit code of the program.
public:
    /**
     * @brief A public method to set the value of the private variable "exit_code_".
     *
     * @param exitCode An integer representing the value to set the private variable "exit_code_" to.
     */
    inline void setExitCode(int exitCode) {
        exit_code_ = exitCode;
    }
};

/// \brief A C++ class called Main that inherits from EntryPoint.
///
/// This is a C++ class named Main that inherits from EntryPoint. It contains several templated methods, including one
/// that tests whether a value is within a range, one that concatenates a variadic number of strings, and one that
/// counts how many items in a collection match a lambda test.
///
/// It also has a method named PrintNumbers that prints numbers in a given range with a 500ms delay using threads and
/// mutexes to synchronize output. Finally, it has a method named OtherCode that tests the PrintNumbers method with
/// multiple threads, using either the Boost program_options library or a simple command line argument parsing method.
class Main : public EntryPoint {

public:
    /** Constructor for the Main class and the entry point for the program
     *  Initializes the base class with the passed arguments
     *
     * @param argc
     * @param argv
     */
    explicit Main(int argc, const char **argv);

    /** Function to test if a value is in a range
     *
     * @tparam T Any type that supports the comparison operators
     * @param value
     * @param min
     * @param max
     * @return True if value is in the range [min, max], false otherwise
     */
    template<typename T>
    static bool IsInBounds(T value, T min, T max) {
        return value >= min && value <= max;
    }

    /** Concatenate args in variadic template
     *
     * @tparam Args Any type that supports the << operator and has str() method
     * @param args
     * @return String containing all args concatenated
     */
    template<typename... Args>
    static string ConcatenateStrings(const Args &... args) {
        stringstream ss;
        (ss << ... << args);
        return ss.str();
    }

    /// Count how many items in the collection matched the lambda test
    ///
    /// @tparam T The type of the collection
    /// @tparam U The type of the lambda test
    /// @param test The lambda test
    /// @param collection The collection to test
    /// @return The number of items in the collection that matched the lambda test
    template<typename T, typename U>
    static int64_t ContainsTheString(U test, vector<T> collection) {
        return count_if(collection.begin(), collection.end(), test);
    }

    /// Print numbers and the thread id in a given range
    /// This method uses a mutex to synchronize the output
    ///
    /// @param start The start number
    /// @param end The end number
    void PrintNumbers(int start, int end) {
        for (int i = start; i <= end; i++) {
            m.lock();
            cout << " " << this_thread::get_id() << "\t\t| " << i << endl;
            m.unlock();
            this_thread::sleep_for(milliseconds(500));
        }
    }

    /// This method is used to test the PrintNumbers method with multiple threads
    /// It will create threads and call PrintNumbers with different parameters
    /// that comes from the command line --start <number> and --end <number>.
    /// The method will wait for all threads to finish before returning.
    /// There is Boost program_options library that is used to parse the command line arguments
    /// If the library is not available, the method will use the old way of parsing the command line arguments
    ///
    /// @return The exit code
    int OtherCode() {
#ifdef HAS_BOOST
        /// Boost program options example
        int start, end;
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "produce help message")
                ("start", po::value<int>(&start)->required(), "starting number")
                ("end", po::value<int>(&end)->required(), "ending number");

        po::variables_map vm;
        try {
            po::store(po::parse_command_line(argc,argv,desc), vm);
            if (vm.count("help")) {
                cout << desc << endl;
                return 0;
            }
            po::notify(vm);
        }
        catch (const exception &e) {
            cerr << e.what() << endl;
            return 1;
        }
#else
        int start = -1;
        int end;

        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i - 1], "--end") == 0) {
                int int1 = strtol(argv[i], nullptr, 10);
                if (int1 > start) {
                    end = int1;
                }
            }
            if (strcmp(argv[i - 1], "--start") == 0) {
                int int1 = strtol(argv[i], nullptr, 10);
                start = int1;
            }
        }

        if (start == -1 || end == -1) {
            cout << "Usage: --start <number> --end <number>" << endl;
            return 1;
        }
#endif

        /// Thread pool example
        vector<thread> threads;
        threads.reserve(5);

        cout << "Thread id\t| Counting Number " << endl;
        for (int i = 0; i < 5; i++) {
            threads.emplace_back(&Main::PrintNumbers, this, start, end);
        }

        for (auto &thread: threads) {
            thread.join();
        }

        return 0;
    }

private:
    shared_mutex m;

};

Main::Main(int p_argc, const char **p_argv) : EntryPoint(p_argc, p_argv) {
    /// Dynamically allocate each type of job.
    std::unique_ptr<Job> pJob = std::make_unique<Job>("Job",
                                                      "Specific task or set of tasks that is completed by an individual in exchange for compensation. This can include args wide range of work, from manual labor to highly skilled professions.",
                                                      40);
    std::unique_ptr<Job> pProgrammer = std::make_unique<Programmer>("Programmer",
                                                                    "Designing, writing, testing, and maintaining computer programs and applications. Programmers work with software development teams to create code, debug programs, and resolve any errors that arise. They may work on various software applications, including operating systems, games, mobile apps, and desktop applications.",
                                                                    40);
    std::unique_ptr<Job> pPilot = std::make_unique<Pilot>("Pilot",
                                                          "Operating aeronautical vehicles such as airplanes and helicopters. Pilots are responsible for the safe operation of the aircraft, including takeoff, flight, and landing. They must also monitor and respond to changes in weather conditions, air traffic control directives, and any mechanical issues that arise.",
                                                          40);

    /// Store each instance in args collection.
    vector<std::unique_ptr<Job>> jobs;
    jobs.push_back(std::move(pJob));
    jobs.push_back(std::move(pProgrammer));
    jobs.push_back(std::move(pPilot));

    /// Iterate the collection and call each of the available interface functions.
    for (auto &&job: jobs) {
        cout << job->name() << endl;
        cout << job->description() << endl;
        cout << job->hours() << endl;
        job->DoWork();
    }

    /// Create args template function that will test if the provided values are within args range. Call the function
    /// and print out the inputs and result. The following should execute assuming httpResonse is args uint32_t value:
    ///     IsInBounds<uint32_t>(httpResonseCode, 500, 599)
    uint32_t httpResonseCode = 500;
    uint32_t min = 500;
    uint32_t max = 599;
    cout << "IsInBounds<uint32_t>(httpResonseCode, " << min << ", " << max << ") = "
         << IsInBounds<uint32_t>(httpResonseCode, min, max) << endl;

    /// Create args function that will take args vector of strings and args test function, It should return how many
    /// items in the collection matched the test. Then print out the result.
    ///     i.  auto theStrings = vector<std::string> { “one”, “two”,  “test”};
    ///     ii. auto count = ContainsTheString([](const std::string& tested) { return tested == “test” }, theStrings);

    auto theStrings = vector<std::string>{"one", "two", "test"};
    auto count = Main::ContainsTheString([](const std::string &tested) { return tested == "test"; }, theStrings);
    cout << "count = " << count << endl;

    /// Write any other code you feel would show off your understanding of C/C++ and programming in general.
    setExitCode(OtherCode());

    /// Bonus: Variadic Template /w usage.
    string resultString = Main::ConcatenateStrings(string("Hello"), string(" "), string("world!"));
    cout << resultString << endl;

    /// Please add passing unit tests.
    //       Check `UnitTest::RunAllTests()` for unit tests

    /// All The source can be in 1 file, it should compile and there should be no runtime errors, and no memory leaks.
    /// Use built in types freely to help you manage resources.

}

/// Unit tests
class UnitTest : public EntryPoint {
public:
    /// Constructor
    UnitTest(int p_argc, const char **p_argv) : EntryPoint(p_argc, p_argv) {
        setExitCode(UnitTest::RunAllTests());
    }

    static int RunAllTests() {
        /// Test Main class
        int argc = 1, argc2 = 5;
        const char *argv[] = {"main"};
        const char *argv2[] = {"main", "--start", "20", "--end", "30"};
        Main main1(argc, argv);
        assert((int) main1 == 1);
        Main main2(argc2, argv2);
        assert((int) main2 == 0);


        /// Test Job class
        unique_ptr<Job> job1 = make_unique<Programmer>("Programmer 1", "writing code", 40);
        unique_ptr<Job> job2 = make_unique<Pilot>("Pilot 1", "flying a plane", 60);

        /// Test job interface functions
        assert(job1->name() == "Programmer 1");
        assert(job1->description() == "writing code");
        assert(job1->hours() == 40);

        assert(job2->name() == "Pilot 1");
        assert(job2->description() == "flying a plane");
        assert(job2->hours() == 60);

        /// Store each instance in a collection
        vector<unique_ptr<Job>> jobs;
        jobs.push_back(std::move(job1));
        jobs.push_back(std::move(job2));

        /// Test IsInBounds function
        assert(Main::IsInBounds<uint32_t>(550, 500, 599) == true);
        assert(Main::IsInBounds<int32_t>(-400, 500, 599) == false);
        assert(Main::IsInBounds<int16_t>(600, 500, 599) == false);

        /// Test ContainsTheString function
        auto theStrings = vector<std::string>{"1", "2", "123"};
        int64_t count = Main::ContainsTheString([](const std::string &tested) { return tested == "123"; }, theStrings);
        assert(count == 1);

        /// Test Variadic Template function
        assert(Main::ConcatenateStrings("Concatenate", " all ", "world!", 1, 2, 3) == "Concatenate all world!123");

        /// Cleanup dynamically allocated memory
        jobs.clear();

        return 0;
    }
};

/// This code defines a C++ program's entry point `main()`.
int main(int argc, const char **argv) {

#ifdef UNIT_TESTS
    UnitTest unitTest(argc, argv);
    return (int)unitTest;
#else
    try {
        Main mainInstance(argc, argv);
        return (int) mainInstance;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
#endif

}

#endif //NINJARMM_SCREENING_STEP1_H
