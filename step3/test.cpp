#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

//item a
class Job
{
public:
    Job(const std::string & name, const std::string desc, const uint32_t hours)
        : name_(name)
        , desc_(desc)
        , hours_(hours)
    {
    }

    virtual ~Job() = default;

    //item d.i
    virtual const std::string & name() const
    {
        return name_;
    }

    //item d.ii
    virtual const std::string & desc() const
    {
        return desc_;
    }

    //item d.iii
    virtual uint32_t hours() const
    {
        return hours_;
    }

    //item d.iv
    virtual void doWork () const
    {
        std::cout << "My work involves " << desc_ << std::endl;
    }

    void log()
    {
    };

    template<typename T, typename... Args>
    void log(const T data, const Args... tail)
    {
        std::cout << data;
        log(tail...);
    }

protected:
    std::string name_;
    std::string desc_;
    uint32_t hours_;
};

//item b
class Programmer : public Job
{
public:
    Programmer(const std::string & name, const std::string desc, const uint32_t hours)
        : Job(name, desc, hours)
    {
    }

private:
    friend std::ostream& operator<<(std::ostream & os, const Programmer & programmer);
};

std::ostream& operator<<(std::ostream & os, const Programmer & programmer)
{
    os << "programmer: " << programmer.name();
    return os;
}

//item c
class Pilot : public Job
{
public:
    Pilot(const std::string & name, const std::string desc, const uint32_t hours)
        : Job(name, desc, hours)
    {
    }

};

//item h
template<class Type>
void isInBounds(const Type httpResponseCode, const Type min, const Type max)
{
    std::cout << min << " < " << httpResponseCode << " < " << max << "? "
        << std::boolalpha << (httpResponseCode < max && httpResponseCode > min) << std::endl;
}

//item i
void countItems(
        bool (*function)(const std::string &),
        const std::vector<std::string> & input)
{
    std::cout << std::count_if(input.begin(), input.end(), function) << std::endl;
}

int main()
{
    //item e, f
    std::vector<std::unique_ptr<Job>> jobs;
    jobs.emplace_back(std::make_unique<Programmer>("Bob", "programming C++ solutions", 160));
    jobs.emplace_back(std::make_unique<Pilot>("Alice", "flying around the world", 120));

    //item g
    for (auto & job : jobs)
    {
        std::cout << "name: " << job->name() << std::endl;
        std::cout << "hours: " << job->hours() << std::endl;
        std::cout << "desc: " << job->desc() << std::endl;
        job->doWork();
    }

    isInBounds(10, 20, 30);
    countItems([](const std::string & item){return item.length() == 3;}, {"foobar", "baz", "corge", "qux"});

    //item h
    Programmer bob("Charles", "logging", 100);
    bob.log("this is log number ", 1, " written at ", 1660267691.122, " by ", bob, '\n');
}

