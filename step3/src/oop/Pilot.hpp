#ifndef PILOT_HPP
    #define PILOT_HPP

#include "Job.hpp"

namespace job {

/// Programmer Class
class Pilot : public Job
{
public:
    Pilot(std::string name, std::string description, uint32_t hours)
        // Member initialization list
        : Job(std::move(name), std::move(description), hours)
    {}

    ~Pilot() override = default;

    /// A polymorphic class should suppress copying
    Pilot(const Pilot&) = delete;// Copy Constructor
    Pilot& operator=(const Pilot&) = delete;// Copy Assignment Operator

    /// Member function
    void DoWork() override;
};

}// End namespace job
#endif
