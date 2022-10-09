#include "travel.h"

/*****************************
 *         PUBLIC           *
 *****************************/

/* Print Reports */
void Travel::printEachCost()
{
    for(auto & a: m_cost)
    {
        Travel::print(a.first, a.second);
    }
}

void Travel::printPersonCost(const std::string & key)
{
    Travel::print(key, this->m_cost[key]);
}

void Travel::printTotal()
{
    Travel::print("Total", this->getTotal());
}

float Travel::getTotal()
{
    float total = 0.0;
    for(auto & a: m_cost)
    {
        total += a.second;
    }
    return total;
}

float Travel::getInd(std::string & value)
{
    return this->m_cost[value];
}

/*****************************
 *         PRIVATE           *
 *****************************/


void Travel::print(const std::string& name, float cost)
{
    std::cout << std::endl
              << "Name: " << name
              << "  ==>"
              << " Travel expenses: "<< cost <<"€";
}


