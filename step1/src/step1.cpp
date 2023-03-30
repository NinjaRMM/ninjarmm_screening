// step1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Job.h"
#include "step1_functions.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace ninjaone_screening;
using namespace std::string_literals;
using std::unique_ptr; using std::make_unique;
using std::vector;
using std::cout; using std::endl;
//Runs the Job class step 1 section
void job_classes_step();

//Runs the IsInbounds section
void in_bounds_test();

//Runs the ContainsTheString section
void contains_the_string_test();


int main()
{
    job_classes_step();

    in_bounds_test();

    contains_the_string_test();

    return 0;
}


void job_classes_step() {
    vector<unique_ptr<Job>> job_array;
    job_array.emplace_back(new Programmer);
    job_array.emplace_back(new Pilot);
    exercise_job_classes(job_array);
}


void in_bounds_test() {

    auto call_and_print_info
        = [](auto val, auto lower, auto upper) {
        cout << "Test Val " << val << " Lower bound " << lower << " Upper bound " << upper
            << " Result " << std::boolalpha << IsInBounds(val, lower, upper)
            << endl;

    };

    call_and_print_info(0, 1, 2);
    call_and_print_info(2, 1, 3);
    call_and_print_info(3ul, 1ul, 4ul);
    call_and_print_info(0.5, 0.2, 0.9);
    //string comparison using std user defined literals
    call_and_print_info("Beta"s, "Alpha"s, "Charlie"s);

}

void contains_the_string_test() {
    auto theStrings = vector<std::string>{ "one", "two",  "test"};
    auto count = ContainsTheString([](const std::string& tested) {
        return tested == "test";
        }, theStrings);

    cout << "Matching Strings: " << count << endl;
}
