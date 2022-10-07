#ifndef TRAITS_HPP
    #define TRAITS_HPP

#if defined(_WIN32) || defined(__CYGWIN__)
    #define WINDOWS_OS
#elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__MACH__)
    #define UNIX_OS
#endif

// Standard Library Headers
#include <array>
#include <iostream>
#include <memory>
#include <string>

namespace osinfo {

#if defined(WINDOWS_OS)

using namespace std::string_literals;

struct Traits {
    inline static void Execute(const std::string& cmd) {
        system(cmd.c_str());
    }

    inline static void GetOsInfo() {
        const auto GET_OS_INFO = R"(powershell "& Get-ComputerInfo -Property *version")"s;
        Execute(GET_OS_INFO);
    }

    inline static void GetOsPatches() {
        const auto GET_OS_PATCHES = R"(powershell "& Get-Hotfix")"s;
        Execute(GET_OS_PATCHES);
    }
};

#elif defined(UNIX_OS)

struct Traits {
    inline static void Execute(const char* cmd) {
        system(cmd);
    }

    inline static void GetOsInfo() {
        const auto GET_OS_INFO = "uname -r -a";
        Execute(GET_OS_INFO);
    }

    inline static void GetOsPatches() {
#if defined(__APPLE__)
        const auto GET_OS_PATCHES = "/usr/sbin/system_profiler SPInstallHistoryDataType";
#else
        const auto GET_OS_PATCHES = "cat /var/log/dpkg.log";
#endif
        Execute(GET_OS_PATCHES);
    }
};

#else
#error "OS not supported"
#endif

}// End namespace osinfo
#endif
