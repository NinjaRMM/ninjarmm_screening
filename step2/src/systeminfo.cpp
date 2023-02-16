/*
Author: Joao Ferreira  (devcicerojr@gmail.com)
Ninjarmm_screening Step 2 : System Information
Validated WINDOWS behavior on my personal laptop and LINUX on website "https://www.onlinegdb.com"
Could not validate MAC behavior, as I don't have a MAC OS environment.

Built  locally (Windows) using CL.exe from Visual Studio MSVC tools
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#ifdef _WIN32
#include "Windows.h"
#elif __APPLE__
#include <sys/sysctl.h>
#include <mach/mach.h>
#endif


std::string getSysInfo() {
    std::stringstream ss;
    std::string line;
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    ss << "Operating System: " << (sysInfo.wProcessorArchitecture == 0 ? "x86" : "x64") << "\n";
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);
    GlobalMemoryStatusEx(&memInfo);
    ss << "Total Memory: " << memInfo.ullTotalPhys / (1024 * 1024) << " MB" << "\n";

#elif __linux__
    std::ifstream osFile("/proc/sys/kernel/ostype");
    osFile >> line;
    ss << "Operating System: " << line << "\n";
    line.clear();
    std::ifstream memInfo("/proc/meminfo");
    while (std::getline(memInfo, line)) {
        if (line.find("MemTotal:") != std::string::npos) {
            ss << "Total Memory " << line.substr(line.find_last_of(":")) << "\n";
            break;
        }
    }
    
#elif __APPLE__
    char osType[64];
    size_t size = sizeof(osType);
    sysctlbyname("kern.ostype", &osType, &size, nullptr, 0);
    ss << "Operating System: " << osType << "\n";
    
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
    unsigned long long totalMem = vmStats.wire_count + vmStats.active_count + vmStats.inactive_count + vmStats.free_count;
    totalMem *= page_size;
    ss << "Total Memory: " << totalMem / (1024 * 1024) << " MB" << "\n";

#endif
    return ss.str();
}



std::string getInstalledPackages() {
    std::string packages ="";
    std::string command = "";
    std::string output = "";
#ifdef _WIN32
    // Use Windows Installer (MSI) library
    command = "wmic qfe list";
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    HANDLE hRead, hWrite;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        std::cout << "Error creating pipe" << std::endl;
        return "error";
    }
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.dwFlags |= STARTF_USESTDHANDLES;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    if (!CreateProcess(NULL, (LPSTR)command.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        return "Error creating process";
    }
    CloseHandle(hWrite);
    DWORD dwRead;
    char buffer[128];
    while (ReadFile(hRead, buffer, 128, &dwRead, NULL)) {
        output += buffer;
    }
    CloseHandle(hRead);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

#elif __linux__
    command = "apt list --installed";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "Error opening pipe";
    }
    char buffer[128];
    while (!feof(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) != NULL)
            output += buffer;
    }
    pclose(pipe);

#elif __APPLE__
    command = "pkgutil --pkgs";
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "Error opening pipe";
    }
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        output += buffer;
    }
    pclose(pipe);
    return output;

#endif
    packages = output;
    return packages;
}

// As this is not production code, I used https://www.onlinegdb.com to validate functions in Linux
// and used my local Windows system to validate behavior on Windows
// I couldn't validate behavior on MAC as I don't have MAC OS environment.
// If this would be a Production code I would create an abstraction level to hide the specific
// implementation on each type of OS, and use CMAKE and the compiler flags to link to a specific library according to the target architecture and OS,
// as well as which compiler to use.
int main() {
    std::cout << getSysInfo() << "\n----------------------------\n";
    std::cout << getInstalledPackages() << std::endl;
    return 0;
}