#include <iostream> 
#include <string>
#include <string.h>
#include <winsock2.h>//include must before window.h
#include <iphlpapi.h>
#include <windows.h>  

#pragma comment(lib, "iphlpapi.lib")

#pragma warning(disable: 4996)//avoid GetVersionEx to be warned

//***** global macros *****//
static const int kMaxInfoBuffer = 256;
#define  GBYTES  1073741824  
#define  MBYTES  1048576  
#define  KBYTES  1024  
#define  DKBYTES 1024.0  

//---- get os info ----//
void getOsInfo()
{
	//get os name according to version number
	OSVERSIONINFO osver = { sizeof(OSVERSIONINFO) };
	GetVersionEx(&osver);
	std::string os_name;
	if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)
		os_name = "Windows 2000";
	else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
		os_name = "Windows XP";
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0)
		os_name = "Windows 2003";
	else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
		os_name = "windows vista";
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)
		os_name = "windows 7";
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
		os_name = "windows 10";

	std::cout << "os name: " << os_name << std::endl;
	std::cout << "os version: " << osver.dwMajorVersion << '.' << osver.dwMinorVersion << std::endl;

}

//---- get cpu info ----//

#ifdef _WIN64

//method 2: usde winapi, works for x86 and x64
#include <intrin.h>
void getCpuInfo()
{
	int cpuInfo[4] = {-1};
	char cpu_manufacture[32] = { 0 };
	char cpu_type[32] = { 0 };
	char cpu_freq[32] = { 0 };

	__cpuid(cpuInfo, 0x80000002);
	memcpy(cpu_manufacture, cpuInfo, sizeof(cpuInfo));

	__cpuid(cpuInfo, 0x80000003);
	memcpy(cpu_type, cpuInfo, sizeof(cpuInfo));

	__cpuid(cpuInfo, 0x80000004);
	memcpy(cpu_freq, cpuInfo, sizeof(cpuInfo));

	std::cout << "CPU manufacture: " << cpu_manufacture << std::endl;
	std::cout << "CPU type: " << cpu_type << std::endl;
	std::cout << "CPU main frequency: " << cpu_freq << std::endl;
}

#else

//mothed 1: this kind asm embedded in code only works in x86 build
//save 4 register variables
DWORD deax;
DWORD debx;
DWORD decx;
DWORD dedx;

//init cpu in assembly language
void initCpu(DWORD veax)
{
	__asm
	{
		mov eax, veax
		cpuid
			mov deax, eax
			mov debx, ebx
			mov decx, ecx
			mov dedx, edx
	}
}

long getCpuFreq()
{
	int start, over;
	_asm
	{
		RDTSC
		mov start, eax
	}
	Sleep(50);
	_asm
	{
		RDTSC
		mov over, eax
	}
	return (over - start)/50000;
}

std::string getManufactureID()
{
	char manuID[25];
	memset(manuID, 0, sizeof(manuID));

	initCpu(0);
	memcpy(manuID + 0, &debx, 4);//copy to array
	memcpy(manuID + 4, &dedx, 4);
	memcpy(manuID + 8, &decx, 4);

	return manuID;
}

std::string getCpuType()
{
	const DWORD id = 0x80000002;//start 0x80000002 end to 0x80000004
	char cpuType[49];
	memset(cpuType, 0, sizeof(cpuType));

	for (DWORD t = 0; t < 3; t++)
	{
		initCpu(id + t);

		memcpy(cpuType + 16 * t + 0, &deax, 4);
		memcpy(cpuType + 16 * t + 4, &debx, 4);
		memcpy(cpuType + 16 * t + 8, &decx, 4);
		memcpy(cpuType + 16 * t + 12, &dedx, 4);
	}

	return cpuType;
}

void getCpuInfo()
{
	std::cout << "CPU manufacture: " << getManufactureID() << std::endl;
	std::cout << "CPU type: " << getCpuType() << std::endl;
	std::cout << "CPU main frequency: " << getCpuFreq() << "MHz" << std::endl;
}

#endif

//---- get memory info ----//
void getMemoryInfo()
{
	std::string memory_info;
	MEMORYSTATUSEX statusex;
	statusex.dwLength = sizeof(statusex);
	if (GlobalMemoryStatusEx(&statusex))
	{
		unsigned long long total = 0, remain_total = 0, avl = 0, remain_avl = 0;
		double decimal_total = 0, decimal_avl = 0;
		remain_total = statusex.ullTotalPhys % GBYTES;
		total = statusex.ullTotalPhys/GBYTES;
		avl = statusex.ullAvailPhys/GBYTES;
		remain_avl = statusex.ullAvailPhys % GBYTES;
		if (remain_total > 0)
			decimal_total = (remain_total/MBYTES)/DKBYTES;
		if (remain_avl > 0)
			decimal_avl = (remain_avl/MBYTES)/DKBYTES;

		decimal_total += (double)total;
		decimal_avl += (double)avl;
		char  buffer[kMaxInfoBuffer];
		sprintf_s(buffer, kMaxInfoBuffer, "total %.2f GB (%.2f GB available)", decimal_total, decimal_avl);
		memory_info.append(buffer);
	}
	std::cout << memory_info << std::endl;
}

//---- get harddisk info ----//
std::string execCmd(const char *cmd)
{
	char buffer[128] = { 0 };
	std::string result;
	FILE *pipe = _popen(cmd, "r");
	if (!pipe) throw std::runtime_error("_popen() failed!");
	while (!feof(pipe))
	{
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	_pclose(pipe);

	return result;
}

void getHardDiskInfo()
{
	std::string hd_seiral = execCmd("wmic path win32_physicalmedia get SerialNumber");
	std::cout << "HardDisk Serial Number: " << hd_seiral << std::endl;
}

//---- get network info ----//
void getNetworkInfo()
{
	//PIP_ADAPTER_INFO struct contains network information
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long adapter_size = sizeof(IP_ADAPTER_INFO);
	int ret = GetAdaptersInfo(pIpAdapterInfo, &adapter_size);

	if (ret == ERROR_BUFFER_OVERFLOW)
	{
		//overflow, use the output size to recreate the handler
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[adapter_size];
		ret = GetAdaptersInfo(pIpAdapterInfo, &adapter_size);
	}

	if (ret == ERROR_SUCCESS)
	{
		int card_index = 0;

		//may have many cards, it saved in linklist
		while (pIpAdapterInfo)
		{
			std::cout << "---- " << "NetworkCard " << ++card_index << " ----" << std::endl;

			std::cout << "Network Card Name: " << pIpAdapterInfo->AdapterName << std::endl;
			std::cout << "Network Card Description: " << pIpAdapterInfo->Description << std::endl;

			//get IP, one card may have many IPs
			PIP_ADDR_STRING pIpAddr = &(pIpAdapterInfo->IpAddressList);
			while (pIpAddr)
			{
				char local_ip[128] = { 0 };
				strcpy(local_ip, pIpAddr->IpAddress.String);
				std::cout << "Local IP: " << local_ip << std::endl;

				pIpAddr = pIpAddr->Next;
			}

			char local_mac[128] = { 0 };
			int char_index = 0;
			for (int i = 0; i < pIpAdapterInfo->AddressLength; i++)
			{
				char temp_str[10] = { 0 };
				sprintf(temp_str, "%02X-", pIpAdapterInfo->Address[i]);//X for uppercase, x for lowercase
				strcpy(local_mac + char_index, temp_str);
				char_index += 3;
			}
			local_mac[17] = '\0';//remove tail '-'

			std::cout << "Local Mac: " << local_mac << std::endl;

			//here just need the first card info
			break;
			//iterate next
			//pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}

	if (pIpAdapterInfo)
		delete pIpAdapterInfo;
}

//---- get process info ----//
void getProcessInfo()
{
    int pid = GetCurrentProcessId();
   //TODO: cpu and mem usage
    printf("Current Pid: %d\n", pid);
}

int main(int argc, char *argv[])
{
	std::cout << "=== os information ===" << std::endl;
	getOsInfo();

	std::cout << "=== cpu infomation ===" << std::endl;
	getCpuInfo();

	std::cout << "=== memory information ===" << std::endl;
	getMemoryInfo();

	std::cout << "=== harddisk information ===" << std::endl;
	getHardDiskInfo();

	std::cout << "=== network information ===" << std::endl;
	getNetworkInfo();

        printf("=== process information ===\n");
        getProcessInfo();

	return 0;
}