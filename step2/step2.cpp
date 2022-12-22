//Please write small program in C++ to do the following:
//	a. Query for installed OS patches and packages
//	b. Query for general system information 
//	c. Feel free to add any functionality you feel will showcase your abilities. 
//	
//You are free to use whatever compiler/platform you are most comfortable with. You may also use an online C++ compiler such as http://cpp.sh/ if you wish, if you do so please be sure to make a note of it as a comment in your code.
//
//We expect your program to be able to run across Windows, Linux and Mac. 
//
//At the top of you main function, please add a paragraph that explains what developer driven validation you took and what process you would expect to take if we were to bring this code to production. If you are unable to confirm compilation and execution on any particular OS, please state so in the first scentence of this paragraph


#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <array>
#include <memory>
#include <stdio.h>

// IMPORTANT: I had now access to a windows or mac toolchain, so I could not implement and test the features for those system;
// I have tried to define at least the proper interface 

class SystemInfoManager
{
    public:

    virtual std::string GetInstalledSoftware() = 0;

    virtual std::string GetSystemInformation() = 0;
    
    protected:

    virtual std::string ExecSystemCommand(const std::string & command) = 0;

};

#ifdef __unix__

class LinuxSystemInfoManager : public SystemInfoManager
{
    public:

    virtual std::string GetInstalledSoftware();
    
    virtual std::string GetSystemInformation();

    protected:

    virtual std::string ExecSystemCommand(const std::string & command);

};

std::string LinuxSystemInfoManager::GetInstalledSoftware()
{
    return ExecSystemCommand("apt list --installed");
}

std::string LinuxSystemInfoManager::GetSystemInformation()
{
    return ExecSystemCommand("cat /etc/os-release");
}

std::string LinuxSystemInfoManager::ExecSystemCommand(const std::string &command)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

#elif _WIN32

class WindowsSystemInfoManager : public SystemInfoManager
{
    public:

    virtual std::string GetInstalledSoftware();
    
    virtual std::string GetSystemInformation();

    protected:

    virtual std::string ExecSystemCommand(const std::string & command);
};

//TODO: Define the proper implementation for Windows 

#elif __APPLE__

class MacSystemInfoManager : public SystemInfoManager
{
    public:

    virtual std::string GetInstalledSoftware();
    
    virtual std::string GetSystemInformation();

    protected:

    virtual std::string ExecSystemCommand(const std::string & command);
};

//TODO: Define the proper implementation for Mac 

#endif

int main()
{
#if __unix__
    LinuxSystemInfoManager pkgManager;
#elif __WIN32
    WindowsSystemInfoManager pkgManager;
#elif __APPLE__
    MacSystemInfoManager pkgManager;
#endif

    while (1)
    {
        std::cout << "Menu" << std::endl;
        std::cout << "1. Get Installed Software" << std::endl;
        std::cout << "2. Get System Information" << std::endl;
        std::cout << "3. Quit" << std::endl;
        std::cout << "Enter your choice: ";

        int option;
        std::cin >> option;

        switch (option)
        {
        case 1:
            std::cout << pkgManager.GetInstalledSoftware() << std::endl;
            // Code for option 1
            break;
        case 2:
            std::cout << pkgManager.GetSystemInformation() << std::endl;
            // Code for option 2
            break;
        case 3:
            return 0;
        default:
            std::cout << "Invalid choice." << std::endl;
            break;
        }
    }

    return 0;
}
