#include "../Jobs/Pilot.h"
#include "../Jobs/Programmer.h"

#include <memory>
#include <vector>
#include <iostream>

using namespace std;
using namespace ninjarmm_screening_step1;

int main() 
{
    vector<unique_ptr<Job>> jobs;
    jobs.emplace_back(make_unique<Pilot>());
    jobs.emplace_back(make_unique<Programmer>());
    for(auto& job : jobs)
    {
        cout << job->GetName() << (job->CanWorkFromHome() ? " can" : " cannot") << " work from home" << endl;
        job->DoOnboarding();
        job->DoWork();
        job->Promote();
    }
    return 0;
}