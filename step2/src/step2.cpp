#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

//Verify OS
#ifdef _WIN32
    #define OS_CMD_PATCHES          "wmic qfe list full"
    #define OS_CMD_GENERAL_INFO     "systeminfo"
    #define OS_CMD_PAUSE            "pause"
#else
    #define OS_CMD_PATCHES          "apt list --installed"
    #define OS_CMD_GENERAL_INFO     "cat /etc/*-release && lscpu"
    #define OS_CMD_PAUSE            ""
#endif


class osInfo
{
public:
    void getOsPatchesInstalled(void);
    void getOsGeneralInfo(void);
};

void osInfo::getOsGeneralInfo (void)
{
    (void)!system(OS_CMD_GENERAL_INFO);
}

void osInfo::getOsPatchesInstalled (void)
{
    (void)!system(OS_CMD_PATCHES);
}

/*  
*   This software is only compatible with Windows and Linux operating systems (64 bits), 
*   for iOS it was not possible to develop due to lack of experience on the platform.
*   This program is not necessary to pass any parameter, after the message "---GENERAL INFO ---" 
*   the program will show hardware information and operating system version, and after showing 
*   the message "---PATCHES INSTALLED ---" the program will return the installed patches.
*/

int main (int argc, char *argv[])
{
   (void)argc;
   (void)argv;

   osInfo osInfoT;

   cout << "Start Step2 Question" << endl;

   cout << "---GENERAL INFO ---" << endl;
   osInfoT.getOsGeneralInfo();

   cout << endl << "---PATCHES INSTALLED ---" << endl;
   osInfoT.getOsPatchesInstalled();

   (void)!system(OS_CMD_PAUSE);

   return 0;
}
