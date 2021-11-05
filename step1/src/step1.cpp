// step1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <Windows.h>
#include <sysinfoapi.h>

int main()
{
    auto sysInfo = SYSTEM_INFO{}; // https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/ns-sysinfoapi-system_info
    GetSystemInfo(&sysInfo);

    auto tempStr = std::string{};

    switch (sysInfo.wProcessorArchitecture)
    {
    case PROCESSOR_ARCHITECTURE_AMD64:
        tempStr = "x64";
        break;
    case PROCESSOR_ARCHITECTURE_ARM:
        tempStr = "ARM";
        break;
    case PROCESSOR_ARCHITECTURE_ARM64:
        tempStr = "ARM64";
        break;
    case PROCESSOR_ARCHITECTURE_IA64:
        tempStr = "Intel Itanium-based";
        break;
    case PROCESSOR_ARCHITECTURE_INTEL:
        tempStr = "x86";
        break;
    default:
        tempStr = "Unknown";
        break;
    }

    std::cout << "Procesor Architecture: " << tempStr << std::endl;

    std::cout << "Page Size: " << sysInfo.dwPageSize << std::endl;

    /*
    * Do other things with sysInfo members
    */

    return 0;
}
