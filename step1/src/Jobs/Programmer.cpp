#include "Programmer.h"

#include <iostream>


using namespace std;
using namespace ninjarmm_screening_step1;

void Programmer::DoOnboarding() noexcept
{
    // TODO: All do stuff to complete the onboarding
    std::cout << "Programmer completed the onboarding" << std::endl;
}

void Programmer::DoWork() noexcept 
{
    // TODO: All do stuff to complete the work of the day
    std::cout << "Programmer completed the workday " << std::endl;

}

bool Programmer::CanWorkFromHome() const noexcept  
{
    return true;
}

void Programmer::Promote() noexcept 
{
    // TODO: Get the next role in the company and set it
    std::cout << "Programmer has been promoted" << std::endl;
}

string Programmer::GetName() const noexcept  
{
    return "Programmer";
}