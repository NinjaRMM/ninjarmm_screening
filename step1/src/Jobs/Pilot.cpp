#include "Pilot.h"

#include <iostream>


using namespace std;
using namespace ninjarmm_screening_step1;

void Pilot::DoOnboarding() noexcept
{
    // TODO: All do stuff to complete the onboarding
    std::cout << "Pilot completed the onboarding" << std::endl;
}

void Pilot::DoWork() noexcept 
{
    // TODO: All do stuff to complete the work of the day
    std::cout << "Pilot completed the workday " << std::endl;

}

bool Pilot::CanWorkFromHome() const noexcept  
{
    return false;
}

void Pilot::Promote() noexcept 
{
    // TODO: Get the next role in the company and set it
    std::cout << "Pilot has been promoted" << std::endl;
}

string Pilot::GetName() const noexcept  
{
    return "Pilot";
}