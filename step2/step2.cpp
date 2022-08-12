// Step 2 code
/*
  This code was validated on:
  1. Windows 10 64-bit
  2. Linux 64 bit
  3. MACos 12.4

  To use this code in production, I would need to test it for different versions of the operating systems and different 
  vendor distributions in the case of Linux. Code may need to be modified to handle different versions.
*/

#include <iostream>
#include <stdlib.h>

using namespace std;

int main()
{
	int selection = 0;
	string cont;

#if defined(_WIN32) || defined (_WIN64)
        system("CLS");
#elif defined(__linux__) || defined(__APPLE__)
        system("clear");
#endif

	while (selection != 6) {
		cout << "Choices:" << endl;
		cout << "  1.  Get installed patches list." << endl;
		cout << "  2.  Get system info." << endl;
		cout << "  3.  Get memory info." << endl;
		cout << "  4.  Get disk info." << endl;
		cout << "  5.  Get cpu info." << endl;
		cout << "  6.  Exit." << endl;

		cout << "\nSelection: ";
		cin >> selection;

		// Patch Info
		if (selection == 1) {
#if defined(_WIN32) || defined (_WIN64)
			system("CLS");
			cout << "Patch info:" << endl;
			system("wmic qfe | findstr KB");
#elif defined(__linux__)
			system("clear");
			cout << "Patch info:" << endl; 
			system("rpm -qa --last");
#elif defined(__APPLE__)
			system("clear");
			cout << "Patch info:" << endl;
			system("system_profiler SPInstallHistoryDataType");
#endif
		}
		// System Info
		else if (selection == 2) {
#if defined(_WIN32) || defined (_WIN64)
			system("CLS");
			cout << "System info:" << endl;
			system("systeminfo");
#elif defined(__linux__)
			system("clear");
			cout << "System info:" << endl;
			system("uname -a");
			cout << endl;
			system("lscpu");
			cout << endl;
			system("lsusb");
			cout << endl;
			system("lspci");
			cout << endl;
			system("lspcmcia");
			cout << endl;
			system("lsdev");
			cout << endl;
			system("lsscsi");
#elif defined(__APPLE__)
			system("clear");
			cout << "System info:" << endl;
			system("system_profiler SPSoftwareDataType SPHardwareDataType");
#endif
		}
		// Memory Info
		else if (selection == 3) {
#if defined(_WIN32) || defined (_WIN64)
			system("CLS");
			cout << "Memory info:" << endl;
			system("systeminfo | findstr Memory");
#elif defined(__linux__)
			system("clear");
			cout << "Memory info:" << endl;
			system("cat  /proc/meminfo");
#elif defined(__APPLE__)
			system("clear");
			cout << "Memory info:" << endl;
			system("top -l 1 -n 0 | grep Mem");
			cout << endl;
			system("vm_stat");
#endif
		}
		// Disk Info
		else if (selection == 4) {
#if defined(_WIN32) || defined (_WIN64)
			system("CLS");
			cout << "Hard Disk:" << endl;
			system("wmic diskdrive get size, caption");
			cout << "Logical Drives:" << endl;
			system("wmic logicaldisk get size, freespace, caption");
#elif defined(__linux__) 
			system("clear");
			cout << "Disk info:" << endl;
			system("sudo hdparm -I /dev/sd*"); // Assuming we have sudo rights
			cout << endl;
			system("df -k");
#elif defined(__APPLE__)
			system("clear");
			cout << "Disk info:" << endl;
			system("diskutil list");
			cout << endl;
			system("df -k");
#endif
		}
		// CPU Info
		else if (selection == 5) {
#if defined(_WIN32) || defined (_WIN64)
			system("CLS");
			cout << "CPU Info:" << endl;
			system("wmic cpu get caption, deviceid, name, numberofcores, maxclockspeed, status");
#elif defined(__linux__) 
			system("clear");
			cout << "CPU Info:" << endl;
			system("lscpu");
#elif defined(__APPLE__)
			system("clear");
			cout << "CPU Info:" << endl;
			system("sysctl -a | grep cpu");
#endif
		}
		// EXIT
		else if (selection == 6)
			return 0;


#if defined(_WIN32) || defined (_WIN64)
		system("pause");
		system("CLS");
#elif defined(__linux__) || defined(__APPLE__)
		cout << "Press any key to continue . . .";
		cout.flush();
		system("read");
		system("clear");
#endif
	}

	return 0;
}
