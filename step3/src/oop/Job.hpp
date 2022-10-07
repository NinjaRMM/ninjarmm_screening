#ifndef JOB_HPP
    #define JOB_HPP

// Standard Library Headers
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace job {

/// Job Class
class Job
{
protected:
    std::string name_;
    std::string description_;
    uint32_t hours_;

public:
    Job(std::string name, std::string description, uint32_t hours)
        // Member initialization list
        : name_(std::move(name))
        , description_(std::move(description))
        , hours_(hours)
    {}

    virtual ~Job() = default;

    /// Demonstrates default/mandatory implementation in the base class
    virtual std::string Name() {
        return name_;
    }

    virtual std::string Description() {
        return description_;
    }

    virtual uint32_t Hours() {
        return hours_;
    }

    /// Pure virtual function
    /// Must be overridden in derived classes
    virtual void DoWork() = 0;
};

/// Type alias
using VecJob = std::vector<std::unique_ptr<job::Job>>;

}// End namespace job
#endif
