#include "travel.h"

void part4()
{
    Travel travel;

    travel.addData("John", 1);
    travel.addData("Mary", 2.3, 1);
    travel.addData("Tom", 1, 4, 134.2, 9);
    travel.addData("Brad", 10, 12);
    travel.addData("Catherine");
    travel.addData("Rosalia", 30, 10040.6);

    std::cout << std::endl << std::endl << "---> Individual Costs";
    travel.printEachCost();

    std::cout << std::endl << std::endl << "---> Concrete Cost";
    travel.printPersonCost("Rosalia");

    std::cout << std::endl << std::endl << "---> Total Costs";
    travel.printTotal();
}

int main() {
    part4();
    return 0;
}
