#include <iostream>
#include <memory>
#include <vector>
#include <cassert>


//create a template function that will test if the provided values are within a range
template<typename T>
bool IsInBounds(T value, T min, T max) {
    return value >= min && value <= max;
}

//create a function that will take a vector of strings and a test function
template<typename T>
int ContainsTheString(T test, std::vector<std::string> theStrings) {
    int count = 0;
    for (auto str : theStrings) {
        if (test(str)) {
            count++;
        }
    }
    return count;
}

/*Class Job*/
class Job {
public:
    Job();
    ~Job();
    //create interface for work job
    virtual void work() = 0;
    //create interface for learning job
    virtual void learn() = 0;
    //create interface for testing job
    virtual void test() = 0;
    //create interface for documenting job
    virtual void document() = 0;
};

Job::Job() {}

Job::~Job() {
    std::cout << "Job is destroyed" << std::endl;
}



/*Class Programmer*/
class Programmer : public Job {
public:
    Programmer();
    ~Programmer();

    void work();

    void learn();

    void test();

    void document();
};

Programmer::Programmer() {}

Programmer::~Programmer() {
    std::cout << "Programmer is destroyed" << std::endl;
}
//implement interface for work job
void Programmer::work() {
    std::cout << "Programmer is working" << std::endl;
}
//implement interface for learning job
void Programmer::learn() {
    std::cout << "Programmer is learning" << std::endl;
}
//implement interface for testing job
void Programmer::test() {
    std::cout << "Programmer is testing" << std::endl;
}
//implement interface for documenting job
void Programmer::document() {
    std::cout << "Programmer is documenting" << std::endl;
}


/*Class Pilot*/
class Pilot : public Job {
public:
    Pilot();
    ~Pilot();
    void work();
    void learn();
    void test();
    void document();
};

Pilot::Pilot() {}

Pilot::~Pilot() {
    std::cout << "Pilot is destroyed" << std::endl;
}
//implement interface for work job
void Pilot::work() {
    std::cout << "Pilot is working" << std::endl;
}
//implement interface for learning job
void Pilot::learn() {
    std::cout << "Pilot is learning" << std::endl;
}
//implement interface for testing job
void Pilot::test() {
    std::cout << "Pilot is testing" << std::endl;
}
//implement interface for documenting job
void Pilot::document() {
    std::cout << "Pilot is documenting" << std::endl;
}

int main() {

    {
        std::vector<std::shared_ptr<Job>> jobs;
        //allocate programmer
        Programmer *programmer = new Programmer();
        //allocate pilot
        Pilot *pilot = new Pilot();

        //create a collection
        jobs.push_back(std::shared_ptr<Job>(programmer));
        jobs.push_back(std::shared_ptr<Job>(pilot));
        //iterate the collection and call each of the available interface functions
        for (auto job: jobs)
        {
            job->work();
            job->learn();
            job->test();
            job->document();
        }
    }

    //test IsInBounds
    uint32_t httpResonseCode = 500;
    std::vector<std::string> theStrings = {"one", "two", "test"};

    //unit tests
    assert(IsInBounds<uint32_t>(httpResonseCode, 500, 599) == true);
    std::cout<< "Test IsInBounds: " << "passed" << std::endl;
    assert(IsInBounds<uint32_t>(httpResonseCode, 400, 499) == false);
    std::cout<< "Test IsInBounds: " << "passed" << std::endl;
    assert(ContainsTheString([](const std::string &tested) { return tested == "test"; }, theStrings) == 1);
    std::cout<< "Test ContainsTheString: " << "passed" << std::endl;
    assert(ContainsTheString([](const std::string &tested) { return tested == "one"; }, theStrings) == 1);
    std::cout<< "Test ContainsTheString: " << "passed" << std::endl;
    assert(ContainsTheString([](const std::string &tested) { return tested == "two"; }, theStrings) == 1);
    std::cout<< "Test ContainsTheString: " << "passed" << std::endl;

    return 0;

}
