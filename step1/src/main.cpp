#include "tasks.h"
#include "unit_test.h"

int main()
{
    std::cout << "RUNNING UNIT TESTS OF THE DEVELOPED CLASSES..." << std::endl << std::endl;
    runUnitTests();
    std::cout << std::endl << "ALL TESTS HAVE PASSED!" << std::endl << std::endl;

    std::cout << "RUNNING THE STEP 1 REQUIRED TASKS..." << std::endl << std::endl;
    executeRequiredTasks();
    std::cout << std::endl << "ALL THE STEP 1 REQUIRED TASKS WERE EXECUTED!" << std::endl;

    return 0;
}
