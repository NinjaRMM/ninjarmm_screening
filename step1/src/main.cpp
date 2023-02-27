#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <vector>

namespace Test {
    using namespace std;

    template<class _Ty_function, class _Ty_result>
    void Do(string_view testName, _Ty_function func, _Ty_result result) {
        cout << testName << " " << ((func() == result) ? "passed\n" : "failed\n");
    }
}

namespace FirstChallenge {
    //a. Create a class Job
    //b. Create a subclass Programmer
    //c. Create a subclass Pilot
    //d. The job class should provide an interface that can do the following
    //  i. Get the name of the job
    //  ii. Get the description of the job.
    //  iii. Get the number of hours required by the job
    //  iv. A DoWork function that prints out “My work involves ” + the description.
    //e. Dynamically allocate each type of job.
    //f. Store each instance in a collection.
    //g. Iterate the collection and call each of the available interface functions.

    using namespace std;

    using Printer = function<void(string)>;

    class Job {
    public:
        virtual ~Job() {};

        virtual string getName() const = 0;
        virtual string getDescription() const = 0;
        virtual unsigned int getRequiredHours() const = 0;
        void DoWork(Printer printerFunction) const {
            stringstream text;
            text << "My work involves " << getDescription() << "\n";
            printerFunction(text.str());
        }
    };

    class Programmer : public Job {
    public:
        string getName() const override {
            return "Programmer";
        }
        string getDescription() const override {
            return "write, modify, and test code and scripts that allow computer software and applications to function properly";
        }
        unsigned int getRequiredHours() const override {
            return 8;
        }
    };

    class Pilot : public Job {
    public:
        string getName() const override {
            return "Pilot";
        }
        string getDescription() const override {
            return "prepare flight plans, communicate with air traffic control, operate aircraft along planned routes and during takeoffs and landings, monitor engines, fuel consumption and the other aircraft system during the flight";
        }
        unsigned int getRequiredHours() const override {
            return 10;
        }
    };

    vector<shared_ptr<Job>> createJobs() {
        return {make_shared<Programmer>(), make_shared<Pilot>()};
    }

    template<typename ... Args>
    void print(Args&& ... args) {
        (cout <<  ... << std::forward<Args>(args)) << '\n';
    }

    void executeJobs(const vector<shared_ptr<Job>>& jobs) {
        for (const auto& job : jobs) {
            print("Job name: ", job->getName(), "\n", "Required Hours: ", job->getRequiredHours(), "\n");
            job->DoWork([](auto&& jobWork) {
                print(jobWork);
            });
        }
    }

    void Do() {
        cout << "\n\n\nFIRST CHALLENGE\n\n";
        auto jobs {createJobs()};
        executeJobs(jobs);
    }
}

namespace SecondChallenge {
    //  h. Create a template function that will test if the provided values are within a range. Call the function and print out the inputs and result. The following should execute assuming httpResonse is a uint32_t value:
    //      i. IsInBounds<uint32_t>(httpResonseCode, 500, 599)

    using namespace std;

    template<typename _Ty>
    bool IsInBounds(_Ty code, decltype(code) minRange, decltype(code) maxRange) {
        return (code >= minRange) && (code <= maxRange);
    }

    void Do() {
        cout << "\n\n\nSECOND CHALLENGE\n\n";

        uint32_t httpResponseCode = 404;
        bool isInBounds {IsInBounds(httpResponseCode, 500, 599)};

        cout << "check if " << httpResponseCode << " is in bounds: " << std::boolalpha << isInBounds << "\n";

        cout << "\nStarting tests\n";
        Test::Do("test1", []{ return IsInBounds<uint32_t>(500, 500, 599); }, true);
        Test::Do("test2", []{ return IsInBounds<int32_t>(599, 500, 599); }, true);
        Test::Do("test3", []{ return IsInBounds<int16_t>(600, 500, 599); }, false);
        Test::Do("test4", []{ return IsInBounds(499, 500, 599); }, false);
        Test::Do("test5", []{ return IsInBounds(-1, 500, 599); }, false);
        Test::Do("test6", []{ return IsInBounds(500, 1, -1); }, false);

    }
}

namespace ThirdChallenge {
    //    i. Create a function that will take a vector of strings and a test function, It should return how many items in the collection matched the test. Then print out the result.
    //      i. auto theStrings = vector<std::string> { “one”, “two”,  “test”};
    //      ii. auto count = ContainsTheString([](const std::string& tested) { return tested == “test” }, theStrings);

    using namespace std;

    uint32_t ContainsTheString(std::function<bool(const string&)> testFunc, const vector<string>& strings) {
        return count_if(strings.begin(), strings.end(), testFunc);
    }

    void Do() {
        cout << "\n\n\nTHIRD CHALLENGE\n\n";

        auto theStrings = vector<string> { "one", "two", "test"};
        auto count = ContainsTheString([](const string& tested) { return tested == "test"; }, theStrings);

        if (count == 0) {
            cout << "the string is not in the vector\n";
        } else if (count == 1) {
            cout << "the string only appears once in the vector\n";
        } else {
            cout << "there are " << count << " appearances of this string in the vector";
        }


        cout << "\nStarting tests\n";
        Test::Do("test1",
                 []{ return ContainsTheString([](const string& tested) {
                     return tested == "test"; }, { "one", "two", "three"}); },
                 0);

        Test::Do("test2",
                 []{ return ContainsTheString([](const string& tested) {
                     return tested == "test"; }, { "test", "one", "two", "test"}); },
                 2);
        Test::Do("Test3",
                 []{ return ContainsTheString([](const string& tested) {
                     return tested == "test"; }, { "ntest", "teste", "one", "two", "test"}); },
                 1);
    }
}

int main() {

    FirstChallenge::Do();
    SecondChallenge::Do();
    ThirdChallenge::Do();

    return 0;
}