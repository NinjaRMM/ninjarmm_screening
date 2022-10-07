#ifndef POLYMORPHIC_HPP
    #define POLYMORPHIC_HPP

// Standard Library Headers
#include <algorithm>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

namespace mpl {

/// Concept 'JobLike'
template <typename T>
concept JobLike = requires(T t)
{
    // 'Interface' that must be implemented by T
    t.Name();
    t.Description();
    t.Number();
    t.DoWork();
};

/// Template Class Job
template <JobLike T>
class Job : public T
{
public:
    // Ctor
    Job() = default;
    Job(std::string name, std::string descr, int number)
        : T{std::move(name), std::move(descr), number}
    {}
};

/// Class Pilot
class Pilot
{
    std::string name_;
    std::string descr_;
    int number_{0};
public:
    // Ctor
    Pilot() = default;
    // Ctor
    Pilot(std::string name, std::string descr, int number);
    // Member Functions
    void Name() const;
    void Description() const;
    void Number() const;
    void DoWork() const;
};

/// Class Programmer
class Programmer
{
    std::string name_;
    std::string descr_;
    int number_{0};
public:
    // Ctor
    Programmer() = default;
    // Ctor
    Programmer(std::string name, std::string descr, int number);
    // Member Functions
    void Name() const;
    void Description() const;
    void Number() const;
    void DoWork() const;
};

/// Type Aliases
using PilotT = Job<Pilot>;
using ProgrammerT = Job<Programmer>;

using TypesT = std::variant<PilotT, ProgrammerT>;
using VectorT = std::vector<TypesT>;

/// Iterate over a collection of 'JobLike' objects
void DisplayJobs(VectorT& jobs);

}// End namespace mpl
#endif
