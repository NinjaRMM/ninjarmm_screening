#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

/**
 * Ye old abstract class
 */
struct Job {
public:
    Job();
    Job(const std::string &name, const std::string &description, uint32_t weeklyHours = 40);
    virtual ~Job() = default;

    /**
     * Define our Getters. They are virtual but as its unlikely
     * the behavior would need to change for a simple getter they
     * dont HAVE to be. This just allows for derived classes to
     * override in future without needing to change the base class
     * which may have MANY descendants.
     */
    virtual const std::string &GetName();
    virtual const std::string &GetDescription();
    virtual uint32_t GetWeeklyHours();

    /** behaviour */
    virtual void DoWork();

protected:
    /**
     * Lets provide these for our concrete classes
     */
    Job(const Job &other) = default;
    Job(Job &&other); /** lets implement this just to do it ... we could use the default however */

    /**
     * Lets provide these for our concrete classes
     */
    Job &operator = (const Job &other) = default;
    Job &operator = (Job &&other); /** lets implement this just to do it ... we could use the default however */

private:
    std::string _name;
    std::string _description;
    uint32_t _weeklyHours;
};

class Programmer : public Job {
public:
    Programmer();
    virtual ~Programmer() = default;

    /**
     * lets let them copy concrete impl classes, we can use defaults since we have no pointers or classes
     * that do NOT correctly implement move construct (std::string is fine). All the member variables
     * are either primitive types or copy constructable items so the default copy constructor works as well.
     */
    Programmer(const Programmer &other) = default;
    Programmer(Programmer &&other) = default;

    /**
     * lets let them copy concrete impl classes the defaults are fine since
     * std::string implements everything we need
     */
    Programmer &operator = (const Programmer &other) = default;
    Programmer &operator = (Programmer &&other) = default;
};

class Pilot : public Job {
public:
    Pilot();
    virtual ~Pilot() = default;

    /** lets let them copy concrete impl classes */
    Pilot(const Pilot &other) = default;
    Pilot(Pilot &&other) = default;

    /** lets let them copy concrete impl classes */
    Pilot &operator = (const Pilot &other) = default;
    Pilot &operator = (Pilot &&other) = default;
};

/** based on the example call with httpResponse code and the fact that you didnt say WHAT RANGE I am going to make the assumption
    that the range is the second and third arg */
template <class T>
bool IsInBounds(T value, T lowerBound, T upperBound) {
    return (value >= lowerBound) && (value <= upperBound);
}

/** Since I dont like the (real world) function above ... I will write a not so real world one  */
template <typename T, typename... Args>
uint32_t CheckInBounds(T lowerBound, T upperBound, Args... args) {
    uint32_t outOfBounds = 0;

    std::vector<T> values{ args... };
    for (auto value : values) {
        if ((value < lowerBound) || (value > upperBound))
            outOfBounds++;
    }

    return outOfBounds;
}

/**
 * Pass in a function to compare the values in the vector. Use a lambda .... or a function its all good.
 */
uint32_t ContainsTheString(std::function<bool (const std::string &)> functor, const std::vector<std::string> values) {
    uint32_t matches = 0;

    for (auto value : values) {
        matches += functor(value) ? 1 : 0;
    }

    return matches;
}

int main() {
    std::vector<std::unique_ptr<Job>> jobs;

    jobs.push_back(std::unique_ptr<Job>(new Programmer));
    jobs.push_back(std::unique_ptr<Job>(new Pilot));

    std::cout << "_________________________________" << std::endl;
    std::for_each(jobs.begin(), jobs.end(), [] (const std::unique_ptr<Job> &job) {
        std::cout << "Name: " << job->GetName() << std::endl;
        std::cout << "Hours: " << job->GetWeeklyHours() << std::endl;
        std::cout << "Description: " << job->GetDescription() << std::endl;
        job->DoWork(); /** probably should have not done the line above ... but the task said ... call em all so ... */
        std::cout << "_________________________________" << std::endl;
    });
    std::cout << std::endl;

    uint32_t httpResponsCode = 200;
    uint32_t lowerBound = 500;
    uint32_t upperBound = 599;
    std::cout << "Lower Bound: " << lowerBound << std::endl;
    std::cout << "Upper Bound: " << upperBound << std::endl;
    std::cout << "Tested Value: " << httpResponsCode << std::endl;
    if (IsInBounds<uint32_t>(httpResponsCode, lowerBound, upperBound)) {
        std::cout << "Result: In bounds" << httpResponsCode << std::endl;
    } else {
        std::cout << "Result: Not in bounds" << std::endl;
    }
    std::cout << std::endl;

    // Use of the "better" variadic version of the bounds checker
    // it basically checks if ALL of the values are in the bounds
    uint32_t outOfBounds = CheckInBounds(lowerBound, upperBound, 500u, 550u, 201u, 403u);
    if (outOfBounds == 0) {
        std::cout << "Result: ALL values in bounds" << std::endl;
    } else {
        std::cout << "Result: " << outOfBounds << " values are out of bounds" << std::endl;
    }

    static const char * const testValue = "test";
    auto theStrings = std::vector<std::string>{ "one", "two", "test" };
    auto count = ContainsTheString([](const std::string &tested) { return tested == testValue; }, theStrings);
    std::cout << testValue << " is contained in the vector " << count << " times" << std::endl;

    return 0;
}

Job::Job() {}

Job::Job(const std::string &name, const std::string &description, uint32_t weeklyHours) :
    _name(name),
    _description(description),
    _weeklyHours(weeklyHours) {}

Job::Job(Job &&other) :
    _name(std::move(other._name)),
    _description(std::move(other._description)),
    _weeklyHours(other._weeklyHours) {
    /** if you HAD RAW pointers in your you would clear the pointers of the "other" class here
        since you are MOVING the resources (note: contrary to popular belief std::move doesnt do this for you
        the implementation of move construct does) */
}

Job &Job::operator = (Job &&other) {
    _name = std::move(other._name);
    _description = std::move(other._description);
    _weeklyHours = other._weeklyHours; /** I could set other._weeklyHours to 0 just to signify it was "moved" but its kinda pointless as its a value primitive */
    /** as above if this class had pointers (raw pointers .. since std::shared_ptr implemens move), after you copy them set them to nullptr */
    return *this;
}

const std::string &Job::GetName() {
    return _name;
}

const std::string &Job::GetDescription() {
    return _description;
}

uint32_t Job::GetWeeklyHours() {
    return _weeklyHours;
}

void Job::DoWork() {
    std::cout << "My work involves " << GetDescription() << "." << std::endl;
}

Programmer::Programmer() :
    Job("Programmer",
        "spending endless hours staring at a screen and banging on a keyboard",
        60) {

}

Pilot::Pilot() :
        Job("Pilot",
            "spending endless hours staring out a window and drinking coffee",
            80) {

}


