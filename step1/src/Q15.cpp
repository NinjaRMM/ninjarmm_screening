#define NTDDI_VERSION NTDDI_WIN7
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <winsock2.h>//The header file needs to be before windows.h
#include <windows.h>
#include <string>
#include <iostream>
#pragma comment(lib,"ws2_32.lib") 

int main()
{
#ifdef _WIN32
	SYSTEM_INFO sysInfo; // This structure contains information about the current computer: the architecture of the computer, the type of CPU, the number of CPUs in the system, the size of pages and other information.
	OSVERSIONINFOEX osvi;
	GetSystemInfo(&sysInfo);
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	if (GetVersionEx((LPOSVERSIONINFOW)&osvi))
	{
		std::cout << "Operating System Version: " << osvi.dwMajorVersion << " " << osvi.dwMinorVersion << osvi.dwBuildNumber << std::endl;
		std::cout << "Service Pack: " << osvi.wServicePackMajor << " " << osvi.wServicePackMinor << std::endl;
	}

	std::cout << "Processor Architecture: " << sysInfo.wProcessorArchitecture << std::endl;
	std::cout << "Processor Level: " << sysInfo.wProcessorLevel << std::endl;
	std::cout << "Processor Version: " << sysInfo.wProcessorRevision << std::endl;
	std::cout << "Processor mask: " << sysInfo.dwActiveProcessorMask << std::endl;
	std::cout << "Number of processors: " << sysInfo.dwNumberOfProcessors << std::endl;
	std::cout << "Processor Type: " << sysInfo.dwProcessorType << std::endl;
	std::cout << "page size: " << sysInfo.dwPageSize << std::endl;
	std::cout << "application minimum address: " << sysInfo.lpMinimumApplicationAddress << std::endl;
	std::cout << "application maximum address: " << sysInfo.lpMaximumApplicationAddress << std::endl;
	std::cout << "virtual memory allocation granularity: " << sysInfo.dwAllocationGranularity << std::endl;
	std::cout << "OemId: " << sysInfo.dwOemId << std::endl;
	std::cout << "wReserved: " << sysInfo.wReserved << std::endl;
#elif __linux__
	if (system("echo -n 'Linux system info: '; uname -a") == 0)
	{
		std::cout << "Linux system info obtained successfully" << std::endl;
	}
	else
	{
		std::cout << "Failed to obtain Linux system info" << std::endl;
	}

#elif __APPLE__ 
	if (system("system_profiler SPSoftwareDataType SPHardwareDataType") == 0)
	{
		std::cout << "Mac system info obtained successfully" << std::endl;
	}
	else
	{
		std::cout << "Failed to obtain Mac system info" << std::endl;
	}

#endif

	return 0;
}