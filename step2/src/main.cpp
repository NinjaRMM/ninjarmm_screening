#include <cstdio>
#include <iostream>


#if defined _WIN32  || defined _WIN64
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <wuapi.h>

#define DIV 1048576 
#define WIDTH 7
#endif

#ifdef linux
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

#endif

using std::cout;
using std::endl;


void getInfo(){
#if defined _WIN32  || defined _WIN64

	cout << "Windows" << endl;

	 OSVERSIONINFOEX info;
	 ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
	 info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	 GetVersionEx((LPOSVERSIONINFO)&info);

	 printf("Windows version: %u.%u\n", info.dwMajorVersion, info.dwMinorVersion);

	 system("wmic qfe list full");
     system("wmic product get Name,Version,InstallDate");


#elif defined linux
	struct utsname *buf = new utsname;
	uname(buf);

	cout << "Linux" << endl;
	cout << "Distro: " << buf->sysname << endl;
	cout << "Release: " << buf->release << endl;
	cout << "Version: " << buf->version << endl;
	cout << "Machine: " << buf->machine << endl;

/*
	To know which packages are installed we need to know first what are the package manager for the distro, 
    so this program wont run on any distro, in this case I'll use apt, but it can be pacman/yum or other.
*/
	system("apt list --upgradable -a");
	system("apt list --installed");


#elif defined __APPLE__

	cout << "MacOS" << endl;
// Im not able to test it, just a ctrl-c ctrl-v from stackoverflow, but it should work.
	char osversion[32];
    size_t osversion_len = sizeof(osversion) - 1;
    int osversion_name[] = { CTL_KERN, KERN_OSRELEASE };

    if (sysctl(osversion_name, 2, osversion, &osversion_len, NULL, 0) == -1) {
        printf("sysctl() failed\n");
        return 1;
    }

    uint32_t major, minor;
    if (sscanf(osversion, "%u.%u", &major, &minor) != 2) {
        printf("sscanf() failed\n");
        return 1;
    }

    if (major >= 20) {
        major -= 9;
        printf("%u.%u\n", major, minor);
    } else {
        major -= 4;

        printf("10.%u.%u\n", major, minor);
    }

	system("softwareupdate --history --all");
	system("pkgutil --pkgs");

#endif

}

int main ( int argc, char ** argv ) {
	getInfo();
	return 0;
}
