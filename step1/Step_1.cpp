/*
    Luis Marquez Ordaz                    luis.marquez.ordaz@gmail.com
    Embedded System Engineer

    Code designed and tested under Visual Studio.

    Compiler: c++14

*/



#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include <stdio.h>

//a
class Job {
private:
    std::string name_;
    std::string description_;
    int hours_;

public:
    Job(const std::string& name, const std::string& description, int hours)
        : name_(name), description_(description), hours_(hours) {}

    virtual ~Job() {}

    //d
    std::string getName() const { return name_; }
    std::string getDescription() const { return description_; }
    int getHours() const { return hours_; }

    virtual void doWork() {
        std::cout << "My work involves:  " << description_ << std::endl;
    }


};

//b
class Programmer : public Job {
public:
    Programmer(const std::string& name, const std::string& description, int hours) : Job(name, description, hours) {};

    virtual ~Programmer() {};

    void doWork() override {
        Job::doWork();
    }
};

//c
class Pilot : public Job {
public:
    Pilot(const std::string& name, const std::string& description, int hours) : Job(name, description, hours) {};

    virtual ~Pilot() {};

    void doWork() override {
        Job::doWork();
    }
};

// h
template <typename T>
bool IsInBounds(T res_code, T lower_b, T upper_b) {
    return (res_code >= lower_b && res_code <= upper_b);
}

//i

int ContainsTheString(std::function<bool(const std::string&)> testFunc, std::vector<std::string>& string_v){
    int count = 0;
    for (auto& element : string_v) {
        if (testFunc(element)) {
            count++;
        }

    }

    return count;
}


int main() {
    std::cout << "  Main () "<< std::endl;

    /*Job Job_0("Scott", "me doing some work", 50);
    Programmer Programmer_0("Henry", "me programing stuff", 50);
    Pilot pilot_0("Henry","me flying airplanes",50);
    pilot_0.doWork();*/
    //std::cout << pilot_0.getDescription();

    std::cout << " Step 1 - a,b,c,d,e,f,g  \n" << std::endl;

    //e-f
    std::vector<Job*> jobs_v;
    jobs_v.push_back(new Programmer("Scott", "me programing stuff", 50));
    jobs_v.push_back(new Pilot("Henry", "me flying airplanes", 20));

    //g
    for (auto job : jobs_v) {
        std::cout << "Name: " << job->getName() << std::endl;
        std::cout << "Description: " << job->getDescription() << std::endl;
        std::cout << "Hours: " << job->getHours() << std::endl;
        job->doWork();
        std::cout << std::endl;
    }
    for (auto job : jobs_v) {
        delete job;
    }


    // h
    std::cout << " Step 1 - h \n" << std::endl;

    std::uint32_t httpResponseCode = 504;
    std::uint32_t Lower_bound = 500;
    std::uint32_t Upper_bound = 599;

    bool req_in_bound = IsInBounds<uint32_t>(httpResponseCode, 500, 599);

    std::cout << "IsInBounds<uint32_t>(httpResonseCode, 500, 599) = "<< std::boolalpha << req_in_bound << std::endl;

    // i
    std::cout << " Step 1 - i \n" << std::endl;

    auto test_string = std::string("test");
    auto theStrings = std::vector<std::string>{ "one", "two", "test" , "test", "test" };

    auto count = ContainsTheString([test_string](const std::string& tested) { return tested == test_string; }, theStrings);

    std::cout << "count: " << count << std::endl;


    std::cout << "\n\n\n   Main () End " << std::endl;

   /* 
    Visual studio and compiler info check:
   
    printf("_MSC_VER : %d \n", _MSC_VER);
    printf("_MSC_FULL_VER : %d \n", _MSC_FULL_VER);
    printf("_MSC_BUILD : %d \n", _MSC_BUILD);

    #ifdef _MSVC_LANG
        printf("_MSVC_LANG : C++%d \n", (_MSVC_LANG / 100) % 2000);
    #endif


    _MSC_VER : 1935
    _MSC_FULL_VER : 193532215
    _MSC_BUILD : 0
    _MSVC_LANG : C++14

    */

}