#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <type_traits>

template <typename T>
std::enable_if_t<std::is_arithmetic_v<T>, bool>
is_in_bounds(T value, T lower_bound, T upper_bound) 
{
    return value >= lower_bound && value <= upper_bound;
}

int count_matches(const std::vector<std::string>& operable_items, std::function<bool(const std::string&)> callback) 
{
    int count = 0;
    for (const auto& item : operable_items) 
    {
        if (callback(item)) 
        {
            count++;
        }
    }
    return count;
}

template<typename T>
void print_args(T arg) {
    std::cout << arg << std::endl;
}

template<typename T, typename... Args>
void print_args(T arg, Args... args) {
    std::cout << arg << " ";
    print_args(args...);
}

class Job
{
protected:
    explicit Job(const std::string& name, const std::string& description, int hoursRequired)
        : name_(name), description_(description), hours_required_(hoursRequired) {}
public:
    virtual ~Job() = default;

    std::string get_name() const { return name_; }
    std::string get_description() const { return description_; }
    int get_hours_required() const { return hours_required_; }

    virtual void DoWork() const { std::cout << "My work involves " << description_ << std::endl; }

private:
    std::string name_;
    std::string description_;
    int hours_required_;
};

class Programmer : public Job
{
    static constexpr int PROGRAMMER_HOURS_REQUIRED = 40;
public:
    Programmer() : Job("Programmer", "writing code", PROGRAMMER_HOURS_REQUIRED) {}
    
    void program() const { std::cout << "I'm programming" << std::endl; }
};

class Pilot : public Job
{
    static constexpr int PILOT_HOURS_REQUIRED = 28;
public:
    Pilot() : Job("Pilot", "flying a plane", PILOT_HOURS_REQUIRED) {}

    void ride_a_plane() const { std::cout << "I'm flying a plane" << std::endl; }
};
