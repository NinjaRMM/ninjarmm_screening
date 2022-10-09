#if defined(__APPLE__)

#include "SystemInfo.hpp"
#include <sys/sysctl.h>


static std::string stringFromSysctlbyname(const std::string& name) {
    char buffer[256];
    size_t bufferSize = sizeof(buffer);
    if (sysctlbyname(name.c_str(), &buffer, &bufferSize, nullptr, 0) != 0) {
        return {};
    }
    return std::string{buffer, bufferSize};
}

std::string SystemInfo::platform() const {
    return std::string{"macOS " + stringFromSysctlbyname("kern.osproductversion")};
}

std::string SystemInfo::cpu() const {
    return stringFromSysctlbyname("machdep.cpu.brand_string");
}

std::string SystemInfo::memory() const {
    int mib[] = {CTL_HW, HW_MEMSIZE};
    int64_t value = 0;
    size_t size = sizeof(value);
    if (sysctl(mib, 2, &value, &size, nullptr, 0) == -1) {
        return 0;
    }
    auto memoryGB = value / 1024 / 1024 / 1024;
    return std::string{std::to_string(memoryGB) + " GB"};
}

#endif
