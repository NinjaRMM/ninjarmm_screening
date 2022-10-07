// Application Headers
#include "Polymorphic.hpp"

namespace mpl {

/// Class Pilot
Pilot::Pilot(std::string name, std::string descr, int number)
    // Member Initialization List
    : name_   {std::move(name)}
    , descr_  {std::move(descr)}
    , number_ {number}
{}

void Pilot::Name() const {
    std::cout << "Name: " << name_ << std::endl;
}
void Pilot::Description() const {
    std::cout << "Description: " << descr_ << std::endl;
}
void Pilot::Number() const {
    std::cout << "Number: " << number_ << std::endl;
}
void Pilot::DoWork() const {
    std::cout << "My work involves: " << descr_ << std::endl;
}

/// Class Programmer
Programmer::Programmer(std::string name, std::string descr, int number)
    // Member Initialization List
    : name_   {std::move(name)}
    , descr_  {std::move(descr)}
    , number_ {number}
{}

void Programmer::Name() const {
    std::cout << "Name: " << name_ << std::endl;
}
void Programmer::Description() const {
    std::cout << "Description: " << descr_ << std::endl;
}
void Programmer::Number() const {
    std::cout << "Number: " << number_ << std::endl;
}
void Programmer::DoWork() const {
    std::cout << "My work involves: " << descr_ << std::endl;
}

/// Iterate over a collection of 'JobLike' objects
void DisplayJobs(VectorT& jobs)
{
    std::for_each(jobs.begin(), jobs.end(), [](const auto& job) {
            std::visit([](const auto& var) {
                var.Name(); var.Description(); var.Number(); var.DoWork();}
                , job);

            std::cout << std::endl;
        });
}

}// End namespace mpl
