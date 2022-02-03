// Joseph A. Urso
// Step 2 of Ninja Screening
// January, 2022
//

#include <iostream>
#include <vector>
#include <string>
#include <functional>

// job collection
std::vector<std::string> jobs;
// test string collection
auto theStrings = std::vector<std::string>{ "one", "two", "test" };
// template declaration
template <typename T>

// in range function, returns true or false if in range or not
bool IsInBounds(T httpResponseCode, T low, T high, T value) {
    // check given value against variables
    if (!(value < low) && (value <= high)) {
        httpResponseCode = 1;
        std::cout << "In range, Response Code: " << httpResponseCode << "\n";
        return true;
    }
    else {
        httpResponseCode = 0;
        std::cout << "Not in range, Response Code: " << httpResponseCode << "\n";
        return false;
    }
}
// string vector match function, returns the number of matches
int ContainsTheString(bool(*tested)(std::string), std::vector<std::string> theStrings) {
    int numMatches = 0;
    // loop through size of string vector theStrings
    for (int i = 0; i < theStrings.size(); i++) {
        // if any element matches "test"
        if (theStrings.at(i) == "test") {
            //increment number of matches
            ++numMatches;
        }
        else {
            continue;
        }
    }
    return numMatches;
}
// primary Job class
class Job {
public:
    // descriptive variables
    std::string name;
    std::string desc;
    std::string hours;
    std::string work;

    // getters and setters
    std::string getName() {
        return name;
    }
    void setName(std::string n) {
        name = n;
    }
    std::string getDesc() {
        return desc;
    }
    void setDesc(std::string d) {
        desc = d;
    }
    std::string getHours() {
        return hours;
    }
    void setHours(std::string h) {
        hours = h;
    }
    // do work function
    std::string DoWork() {
        work = "My work involves " + desc + "\n";

        return work;
    }
    // function to add each job to collection
    void addJob() {
        jobs.push_back(name);
        jobs.push_back(desc);
        jobs.push_back(hours);
        jobs.push_back(work);
    }
};
// programmer subclass
class Programmer : public Job {
public:
    // print functions for testing purposes
    void printName() {
        std::cout << name << "\n";
    }
    void printDesc() {
        std::cout << desc << "\n";
    }
    void printHours() {
        std::cout << hours << "\n";
    }

};
// pilot subclass
class Pilot : public Job {
public:
    // print functions for testing purposes
    void printName() {
        std::cout << name << "\n";
    }
    void printDesc() {
        std::cout << desc << "\n";
    }
    void printHours() {
        std::cout << hours << "\n";
    }

};

int main()
{
    // instances of classes
    Job j;
    Programmer p;
    Pilot pt;
    int rangeValue = 0;
    // set programmer information
    p.setName("Programmer");
    p.setDesc("Code all day");
    p.setHours("40");
    p.DoWork();
    // add to collection
    p.addJob();
    // set pilot information
    pt.setName("Pilot");
    pt.setDesc("Fly all day");
    pt.setHours("50");
    pt.DoWork();
    // add to collection
    pt.addJob();

    std::cout << "***************************\n";
    std::cout << "------Joseph A. Urso-------\n";
    std::cout << "------NinjaOne Step 2------\n";
    std::cout << "***************************\n\n\n";

    std::cout << "First part, iterating job collection...\n";

    // iterate through size of jobs collection, return values
    for (int i = 0; i < jobs.size(); i++) {
        std::cout << jobs.at(i) << "\n";
    }

    std::cout << "Second part, value in range\n";
    std::cout << "Please Enter a Value between 500 & 599\n";
    // get user input for value
    std::cin >> rangeValue;
    // call in bounds function using input
    IsInBounds(0, 500, 599, rangeValue);

    std::cout << "Third and final part, Running string vector match test...\n";
    // declare count for string vector match
    auto count = ContainsTheString([](const std::string tested) { return tested == "test"; }, theStrings);
    // print number of matches
    std::cout << "Number of Matches: " << count << "\n";

    return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

