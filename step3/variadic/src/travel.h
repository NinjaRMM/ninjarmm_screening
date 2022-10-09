#ifndef VARIADICT_TRAVEL_H
#define VARIADICT_TRAVEL_H

#include <iostream>
#include <map>

class Travel
{
public:
    /* Create a key with data costs */
    /* Create a key with data costs */
    template<typename M>
    void addData(M name)
    {
        m_cost[name] = 0.0;
    }
    template<typename M, typename... D>
    void addData(M name, D... costs)
    {
        m_cost[name] = sum_costs(costs...);
    }

    /* Getters */
    float getTotal();
    float getInd(std::string & value);

    /* Print Reports */
    void printEachCost();
    void printPersonCost(const std::string & key);
    void printTotal();

private:
    std::map<std::string, float> m_cost;

    /* Sum travel costs */
    template<typename T>
    float sum_costs(T cost)
    {
        return 1.0 * cost;
    }

    template<typename P, typename... T>
    float sum_costs(P first, T... costs)
    {
        return (first * 1.0) + sum_costs(costs...);
    }

    /* Format results */
    static void print(const std::string& name, float cost);

};

#endif //VARIADICT_TRAVEL_H
