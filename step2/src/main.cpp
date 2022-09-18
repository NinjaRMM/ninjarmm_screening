#include "info.h"

// This program has been validated on:
//  - Windows 10 64-bit
//    - MinGW  9.2
//    - Clang 12.0.1
//    - MSVC 2019 (v142, v141, v140)
//  - Windows 11 64-bit
//    - MinGW 11.2
//    - MSVC 2022
//  - WSL Ubuntu 22.04
//    - GCC 11.2
//  - WSL Ubuntu 20.04
//    - GCC 9.4
//  - Ubuntu 18.04.6
//    - GCC 7.5
//  - Fedora 32
//    - GCC 10.3.1
//  - MacOS Mojave 10.14.6
//    - Clang 11.0
//  - MacOS Big Sur 11.6.4
//    - Clang 13.0
//  - MacOS Big Sur 11.6.6
//    - Clang 12.0.5
//
// To be able to use this program in production, it would need to be tested on
// more operating system versions. Especially in the case of Linux.

int
main() {
    std::vector<std::string> osPatches;

    printf("Os Patches:\n");
    if (QueryOsPatches(osPatches)) {
        for (const auto &str : osPatches) {
            if (str.empty() == false) {
                printf(" - %s\n", str.c_str());
            }
        }
    }

    vecStrStr systemInfo;

    printf("\nSystem Info:\n");
    if (QuerySystemInfo(systemInfo)) {
        // MSVC 2015 does not support structured bindings
        //for (const auto &[key, value] : systemInfo) {
        for (const auto &t : systemInfo) {
            const auto &key   = std::get<0>(t);
            const auto &value = std::get<1>(t);
            if (key[0] != ' ') {
                printf(" - %s: %s\n", key.c_str(), value.c_str());
            }
            else {
                printf("   %s: %s\n", key.c_str(), value.c_str());
            }
        }
    }

    return 0;
}
