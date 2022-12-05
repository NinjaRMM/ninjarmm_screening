/* I don't have any MAC_OS pc available to test right now, so I skipped it's part */


#include <iostream>


#ifdef __linux__ || __unix__
int main()
{
    std::cout << "OS patches: " << std::endl;
    system("cat /etc/os-release");
    std::cout << "OS packages: " << std::endl;
    system("apt list --installed");
    std::cout << "General System Information: " << std::endl;
    system("uname -a");
    system("lscpu");
    return 0;
}   
#elif _WIN32
int main()
{
    std::cout<< "OS patches: " << std::endl;
    system("wmic qfe list");
    std::cout<< "OS Packages: " << std::endl;
    system("Get-AppxPackage -AllUsers | Select Name, PackageFullName");
    std::cout<< "General System Information: " << std::endl;
    system("systeminfo");
    return 0;
}
#endif 
