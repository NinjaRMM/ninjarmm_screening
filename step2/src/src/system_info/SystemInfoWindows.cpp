#if defined(_WIN32)

#include "SystemInfo.hpp"
#include <stdio.h>
#include <windows.h>
#include <intrin.h>


std::string SystemInfo::platform() const {
    DWORD dataSize = 0;
    char buffer[256];
    dataSize = sizeof(buffer);

    RegGetValueA(
        HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
        "ProductName",
        RRF_RT_REG_SZ,
        nullptr,
        &buffer,
        &dataSize);

    return std::string{buffer};
}

std::string SystemInfo::cpu() const {
    int CPUInfo[4] = {-1};
    unsigned nExIds, i = 0;
    char CPUBrandString[0x40];
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    for (i = 0x80000000; i <= nExIds; ++i) {
        __cpuid(CPUInfo, i);
        if (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }
    return std::string{CPUBrandString};
}

std::string SystemInfo::memory() const {
    uint64_t memory = 0;
    GetPhysicallyInstalledSystemMemory(&memory);
    return std::string{std::to_string(memory / 1024 / 1024) + " GB"};
}

std::string SystemInfo::disk() const {
    uint64_t freeBytes = 0;
    uint64_t totalBytes = 0;
    uint64_t totalFreeBytes = 0;
    if (!GetDiskFreeSpaceEx(
            ".",
            (PULARGE_INTEGER) &freeBytes,
            (PULARGE_INTEGER) &totalBytes,
            (PULARGE_INTEGER) &totalFreeBytes)) {
        return {};
    }
    auto totalGB = totalBytes / 1024 / 1024 / 1024;
    return std::string{std::to_string(totalGB) + " GB"};
}

#endif
