// Author: Joao Ferreira   devcicerojr@gmail.com
// Ninja_rmm screening Step 3
// Unit Test framework used: Catch2
// I tested the following code on my local Windows computer.
// I compiled it using MSVC15 tools (cl.exe compiler).
// When the binary .exe file runs it will run all the Test Cases.
// The last Test Case simply runs a "main" function that does
// every steps asked in the challenge requirements.


#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// ITEM h.
template <typename T>
bool isInBounds(T response , T min, T max) {
    bool res = false;
    if (min <= response && response <= max) {
        res = true;
    }
    return res;
}
// -----------------------------------------

// ITEM i.
bool containsTest(const string &str) {
    return  (str.find("test") != std::string::npos) ? true : false;
}

bool containsTheString(bool (*func)(const string &str), const vector<string> &v) {
    bool res = false;
    for (const auto &e : v) {
        if ((*func)(e)) {
            res = true;
            break;
        }
    }
    return res;
}
// -----------------------------------------


// ITEM j.   Template Variadics
template<typename T>
T sum(T arg) {
    return arg;
}

template<typename T, typename... Args>
T sum(T arg, Args... args) {
    return arg + sum(args...);
}
// -----------------------------------------


// ITEMS a.  to  g.

// As the challenge doesn`t require the interface to be able of setting the Name, Description, RequiredHours and doWork string, 
// I could simply have them as member variables of the Job class, declare them as const, and have each derived class` constructor
// to initialize them with appropriate values. I could simply have the Job`s interface to return the member variables like:
//
// string getName() {return name_;}
//
// But it would also remove the need of implementing derived versions of the corresponding functions in Job class.
// Thus, I decided not to follow that approach, as I think the objective of this challenge is to have derived classes
// implementing virtual functions from the Base class.
class Job {
private:
	const string do_work_str_ = "My work involves ";
protected:
	virtual string innerDoWork() const = 0;
public:

	virtual string getName() const = 0;
	virtual string getDescription() const = 0;
	virtual unsigned int reqHours() const = 0;
	void doWork(ostream& os = cout) const { os << do_work_str_ << (this->innerDoWork());};
	
	virtual ~Job() = default;

};

class Programmer : public Job {
protected:
    string innerDoWork() const override  {
        return "design and implement code, guarantee that it is secure, clean and efficient, "
        "as well as review it and maintain it.";
    }
public:
    Programmer() = default;

	string getName() const override {
	    return "Programmer";
	}
	
	string getDescription() const override {
	    return "A Programmer is a professional that is able to think logically and describe how to solve a problem, "
	    "or achieve a specific goal, in a language that is understood by "
	    "a computer, with the help of a compiler or interpreter.";
	}
	
	unsigned int reqHours() const override {
	    return 800;
	}
	
    ~Programmer() = default;
};

class Pilot : public Job {
protected:
    string innerDoWork() const override {
        return "control and drive a vehicle";
    }
public:
    Pilot() = default;

    string getName() const override {
        return "Pilot";
    }
    
    string getDescription() const override {
        return "A Pilot is a professional who is skilled and trained to drive and handle a vehicle.";
    }
    
    unsigned int reqHours() const override {
        return 600;
    }
    
    ~Pilot() = default;
};

// -----------------------------------------
// This function is our 'main'. However, as we have the unit tests with Catch2 framework, the execution of the .exe file will run the test_cases
// Thus, this function was named 'the_main' so that we can test it by calling the appropriate test case.
// The test case that runs this function is called: "Running function the_main() does not throw any exception"
int the_main() {

	// Using ITEMs a to g
    vector<unique_ptr<Job>> jobs;
	jobs.push_back(make_unique<Pilot>());
	jobs.push_back(make_unique<Programmer>());
	
	for(const auto &e : jobs) {
	    cout << e->getName();
	    cout << "\n";
	    cout << e->getDescription();
	    cout << "\n";
	    cout << e->reqHours() << "hrs required\n";
	    e->doWork();
	    cout << "\n\n";
	}
	// -----------------------------------------
	
	return 0;
}

TEST_CASE("Pilot getName") {
	const string pilot_name = "Pilot";
	unique_ptr<Pilot> pilot = make_unique<Pilot>();
	REQUIRE(pilot->getName() == pilot_name);
}

TEST_CASE("Pilot getDescription") {
	const string pilot_descr = "A Pilot is a professional who is skilled and trained to drive and handle a vehicle.";
	unique_ptr<Pilot> pilot = make_unique<Pilot>();
	REQUIRE(pilot->getDescription() == pilot_descr);
}

TEST_CASE("Pilot reqHours") {
	const unsigned int pilot_req_hrs = 600;
	unique_ptr<Pilot> pilot = make_unique<Pilot>();
	REQUIRE(pilot->reqHours() == pilot_req_hrs);
}

TEST_CASE("Pilot doWork") {
	const string pilot_work_str = "My work involves control and drive a vehicle";
	unique_ptr<Pilot> pilot = make_unique<Pilot>();
	ostringstream oss;
	pilot->doWork(oss);
	REQUIRE(oss.str() == pilot_work_str);
}

TEST_CASE("Programmer getName") {
	const string programmer_name = "Programmer";
	unique_ptr<Programmer> programmer = make_unique<Programmer>();
	REQUIRE(programmer->getName() == programmer_name);
}

TEST_CASE("Programmer getDescription") {
	const string programmer_descr = "A Programmer is a professional that is able to think logically and describe how to solve a problem, "
	    "or achieve a specific goal, in a language that is understood by "
	    "a computer, with the help of a compiler or interpreter.";
	unique_ptr<Programmer> programmer = make_unique<Programmer>();
	REQUIRE(programmer->getDescription() == programmer_descr);
}

TEST_CASE("Programmer reqHours") {
	const unsigned int programmer_req_hrs = 800;
	unique_ptr<Programmer> programmer = make_unique<Programmer>();
	REQUIRE(programmer->reqHours() == programmer_req_hrs);
}

TEST_CASE("Programmer doWork") {
	const string programmer_work_str = "My work involves design and implement code, "
		"guarantee that it is secure, clean and efficient, as well as review it and maintain it.";
	unique_ptr<Programmer> programmer = make_unique<Programmer>();
	ostringstream oss;
	programmer->doWork(oss);
	REQUIRE(oss.str() == programmer_work_str);
}

TEST_CASE("isInBounds should return true if passed uint32_t input value is in range") {
	uint32_t input = 50u;
	uint32_t min = 25u;
	uint32_t max = 70u;
	REQUIRE(isInBounds(input, min, max) == true);
}

TEST_CASE("isInBounds should return false if passed uint32_t input value is less than min") {
	uint32_t input = 24u;
	uint32_t min = 25u;
	uint32_t max = 70u;
	REQUIRE(isInBounds(input, min, max) == false);
}

TEST_CASE("isInBounds should return true if passed uint32_t input value is equal to min") {
	uint32_t input = 25u;
	uint32_t min = 25u;
	uint32_t max = 70u;
	REQUIRE(isInBounds(input, min, max) == true);
}

TEST_CASE("isInBounds should return false if passed uint32_t input value is greater than max") {
	uint32_t input = 71u;
	uint32_t min = 25u;
	uint32_t max = 70u;
	REQUIRE(isInBounds(input, min, max) == false);
}

TEST_CASE("isInBounds should return true if passed uint32_t input value is equal to max") {
	uint32_t input = 70u;
	uint32_t min = 25u;
	uint32_t max = 70u;
	REQUIRE(isInBounds(input, min, max) == true);
}

TEST_CASE("isInBounds should return as expected for other types, that are not uint32_t but implement the comparison operators") {
	char c_input = 'h';
	char c_input2 = 'a';
	char c_min = 'e';
	char c_max = 'z';
	REQUIRE(isInBounds(c_input, c_min, c_max) == true);
	REQUIRE(isInBounds(c_input2, c_min, c_max) == false);
}

TEST_CASE("containsTest should return true if the input string contains 'test' as substring") {
	const string input = "thistest";
	REQUIRE(containsTest(input) == true);
}

TEST_CASE("containsTest should return false if the input string does not contain 'test' as substring") {
	const string input = "thisfest";
	REQUIRE(containsTest(input) == false);
}

TEST_CASE("containsTest should return false if the input string is empty") {
	const string input = "";
	REQUIRE(containsTest(input) == false);
}

TEST_CASE("containsTheString should return true if one of the strings provided as input contains 'test' as substring") {
	const vector<string> input = {"one", "two", "test", "four", "five"};
	REQUIRE(containsTheString(&containsTest , input) == true);
}

TEST_CASE("containsTheString should return false if none of the strings provided as input contains 'test' as substring") {
	const vector<string> input = {"one", "two", "three", "four", "five"};
	REQUIRE(containsTheString(&containsTest , input) == false);
}

TEST_CASE("containsTheString should return false if the collection of strings provided as input is empty") {
	const vector<string> input = {};
	REQUIRE(containsTheString(&containsTest , input) == false);
}

TEST_CASE("sum function should return the proper sum of the provided numbers") {
	REQUIRE(sum(1, 2, 3, 4, 5, 6) == 21);
	REQUIRE(sum(1.7f, 3.2f, 0.1f, 3.5f) == 8.5f);
	REQUIRE(sum(-2, 5, 17, -8, 0, 10) == 22);
}

TEST_CASE("Running function the_main() does not throw any exception") {
	REQUIRE_NOTHROW(the_main());
}