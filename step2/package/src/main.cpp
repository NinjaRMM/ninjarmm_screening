#include <iostream>


#ifdef _WIN32
    const bool  known_platform = true;
    const std::string  cmd_package_list = "wmic qfe list full";
    const std::string  cmd_program_list = "wmic product get Name,Version,InstallDate"; 
#elif __linux__
    const bool  known_platform = true;
    const std::string  cmd_package_list = "apt list --upgradable -a";
    const std::string  cmd_program_list = "apt list --installed";
    
#elif __APPLE__
    // Here I would have the cmd for IOS but I don't know
    // I don't have experience in Mac
    const bool  known_platform = false; // true if we fill the cmd for IOS
#else
    const bool  known_platform = false;
#endif //_WIN32




void printPackages(){
    std::cout << "*********     Package List   *************\n";
    system(cmd_package_list.c_str());  
}

void printPrograms()
{
    std::cout <<  "*********     Program List   *************\n";
    system(cmd_program_list.c_str());
}

int main(int, char**) {
    if (known_platform)
    {
        printPackages();
        printPrograms();
    
    }
    else
    {
        std::cout << "Operating System not known!\n";
    }
    
}
