#ifndef PROGRAMMER_HPP
    #define PROGRAMMER_HPP

#include "Job.hpp"

namespace job {

/// Programmer Class
class Programmer : public Job
{
public:
    Programmer(std::string name, std::string description, uint32_t hours)
        // Member initialization list
        : Job(std::move(name), std::move(description), hours)
    {}

    ~Programmer() override = default;

    /// A polymorphic class should suppress copying
    Programmer(const Programmer&) = delete;// Copy Constructor
    Programmer& operator=(const Programmer&) = delete;// Copy Assignment Operator

    /// Member function
    void DoWork() override;
};

}// End namespace job
#endif
