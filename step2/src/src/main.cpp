#include "system_info/SystemInfo.hpp"
#include "os_patch/OSPatches.hpp"

#include <iostream>


//The code was made to be as simple as possible (YAGNI principle), in a short development period. To make this code production-ready, the project requirements must first be verified. After that, it needs to be verified if the information was collected is in agreement and if there are other more efficient ways to obtain it. An interface can be added to classes to decouple their implementation and provide a better alternative for implementing unit tests. Tests can also validate different scenarios, as well as different OS versions, which may not be compatible with current code.

int main (int argc, char *argv[])
{
    std::cout << "## Query for installed OS patches" << std::endl;
    auto osPatches = OSPatches();
    for (const auto& patch : osPatches.get()) {
        std::cout << patch << std::endl;
    }
    std::cout << std::endl;


    std::cout << "## Query for general system information" << std::endl;
    auto systemInfo = SystemInfo();
    std::cout << "Platform:  " << systemInfo.platform() << std::endl;
    std::cout << "CPU:  " << systemInfo.cpu() << std::endl;
    std::cout << "Memory:  " << systemInfo.memory() << std::endl;
    std::cout << "Disk:  " << systemInfo.disk() << std::endl;
    std::cout << std::endl;

    return 0;
}
