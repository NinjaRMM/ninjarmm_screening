/*

This code assumes the commands used to query the system for information are available in the target OS

Linux:
    Tested on Ubuntu 22.04, but any distribution based on Debian will also work
    Commands used:
        uname -r -a
        cat /var/log/dpkg.log

macOS:
    Tested on macOS 12.6 Monterey
    Commands used:
        uname -r -a
        /usr/sbin/system_profiler SPInstallHistoryDataType

 Windows:
    Tested on Windows 10 & 11, but should work on any Windows version that has PowerShell and its scripts installed
    Commands used:
        powershell "& Get-ComputerInfo -Property *version"
        powershell "& Get-Hotfix"


If this code was to be used in a production environment, it would be necessary to check if the commands
are available in the target system, and if not, use other commands or other methods to obtain the information

Making use of native OS API's would be a better option, but it would require more work and time to implement
It would also provide the means to create Unit Tests for the code, which would be a good practice to follow

*/

// Application Headers
#include "osinfo/Traits.hpp"

/// Main entry point for the application
auto main() -> int {
    // Simple lambda to print a message to the console
    auto print = [](const char* str) {std::cout << '\n' << str << '\n' << '\n';};

    print("*** OS Patches ***");
    osinfo::Traits::GetOsPatches();

    print("*** OS Info ***");
    osinfo::Traits::GetOsInfo();

    return 0;
}
