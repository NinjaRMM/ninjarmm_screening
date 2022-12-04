// unable to confirm compilation and execution on MAC

#include <stdio.h>
#include <iostream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(_WIN64)


int main()
{
    std::cout<< "installed OS patches";
    system("powershell -ExecutionPolicy Bypass Get-Hotfix");

    std::cout<< "installed OS Packages";
    system("powershell -ExecutionPolicy Bypass Get-Package");

    std::cout<< "General System Information";
    system("powershell -ExecutionPolicy Bypass Get-ComputerInfo");

    return 0;
}

#elif __APPLE__
int main()
{
    std::cout << "installed OS patches" << std::endl << std::flush;
	system("/usr/bin/env bash -c \"softwareupdate --history --all\"");
	std::cout << std::endl << std::flush;

    std::cout << "installed OS Packages" << std::endl << std::flush;
    system("/usr/bin/env bash -c \"pkgutil --pkgs\"");
	std::cout << std::endl << std::flush;

    std::cout<< "General System Information" << std::endl << std::flush;
    system("/usr/bin/env bash -c \"uname -a\"");
	std::cout << std::endl << std::flush;
    system("/usr/bin/env bash -c \"system_profiler SPSoftwareDataType SPHardwareDataType\"");
	std::cout << std::endl << std::flush;

    return 0;
}
#elif defined(__linux__) || defined(__unix__)

int main()
{
    std::cout << "installed OS patches" << std::endl << std::flush;
    system("/usr/bin/env bash -c \"cat /etc/os-release\"");
	std::cout << std::endl << std::flush;

    std::cout << "installed OS Packages" << std::endl << std::flush;
    system("/usr/bin/env bash -c \"pacman -Qe\"");
	std::cout << std::endl << std::flush;

    std::cout<< "General System Information" << std::endl << std::flush;
    system("/usr/bin/env bash -c \"uname -a\"");
	std::cout << std::endl << std::flush;
    system("/usr/bin/env bash -c \"lscpu\"");
	std::cout << std::endl << std::flush;

    return 0;
}

#else
#   error "Unknown compiler"
#endif
