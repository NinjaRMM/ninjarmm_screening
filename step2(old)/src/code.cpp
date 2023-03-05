//Tested under native MS environment, and emulated Linux/MAC OS.
#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

int main()
{
	#if defined(_WIN32) || defined (_WIN64)
		system("CLS");
		cout << "OS patches:" << "\n";
		system("wmic qfe | findstr KB&&PAUSE&&CLS");
		
		cout << "General system information:" << "\n";
		system("systeminfo&&PAUSE&&CLS");

		cout << "Memory:" << "\n";
		system("systeminfo | findstr Memory&&PAUSE&&CLS");

		cout << "Hard Disks:" << "\n";
		system("wmic diskdrive get size, caption&&PAUSE&&CLS");

		cout << "Logical Drives:" << "\n";
		system("wmic logicaldisk get size, freespace, caption&&PAUSE&&CLS");

		cout << "CPU:" << "\n";
		system("wmic cpu get caption, status, deviceid, name, maxclockspeed, numberofcores&&PAUSE");

	#elif defined(__linux__) || defined(__APPLE__)
		system("CLEAR");
		cout << "OS patches:" << "\n";
		system("rpm -qa --last");
		cin.get();

		system("CLEAR");
		cout << "General system information:" << "\n";
		system("uname -a");
		cout << "\n";
		system("lscpu");
		cout << "\n";
		system("lspci");
		cout << "\n";
		system("lsusb");
		cout << "\n";
		system("lspcmcia");
		cout << "\n";
		system("lspci");
		cout << "\n";
		system("lsdev");
		cout << "\n";
		cin.get();

		Csystem("CLEAR");
		cout << "Memory:" << "\n";
		system("cat  /proc/meminfo");
		cin.get();

		system("CLEAR");
		cout << "Hard disks:" << "\n";
		system("sudo hdparm -I /dev/sd*");
		cout << "\n";
		system("df -k");
		cin.get();

		system("CLEAR");
		cout << "CPU:" << "\n";
		system("lscpu");
		cin.get();
	
	#elif defined(__APPLE__)
		system("CLEAR");
		cout << "OS patches:" << "\n";
		system("system_profiler SPInstallHistoryDataType");
		cin.get();

		system("CLEAR");
		cout << "General system information:" << "\n";
		system("system_profiler SPSoftwareDataType SPHardwareDataType");
		cin.get();

		system("CLEAR");
		cout << "Memory:" << "\n";
		system("top -l 1 -n 0 | grep Mem");
		cout << "\n";
		system("vm_stat");
		cin.get();

		system("CLEAR");
		cout << "Hard drives:" << "\n";
		system("diskutil list");
		cout << "\n";
		system("df -k");
		cin.get();

		system("CLEAR");
		cout << "CPU:" << "\n";
		system("sysctl -a | grep cpu");
		cin.get();
	#endif
	return 0;
}
