#if defined(__APPLE__)

#include "info.h"
#include "info_common.h"

//--------------------------------------
bool
QueryOsPatches(vecStr &strings) {
    if (GetCommandLineOutput("pkgutil --pkgs", strings)) {
        for (auto &str : strings) {
            Trim(str, " \n\r");
        }
        return true;
    }
    return false;
}

//--------------------------------------
static void
GetInfo(vecStrStr &pairs, const std::string &commandLine) {
    vecStr strings;

    if (GetCommandLineOutput(commandLine, strings)) {
        for (const auto &str : strings) {
            vecStr parts = Split(str, ':');
            if (parts.size() == 2) {
                Trim(parts[0], "\" \t\n\r");
                Trim(parts[1], "\" \t\n\r");
                if (parts[1].length() > 0) {
                    pairs.emplace_back(parts[0], parts[1]);
                }
            }
        }
    }
}

//--------------------------------------
bool
QuerySystemInfo(vecStrStr &pairs) {
    GetInfo(pairs, "/usr/sbin/system_profiler SPHardwareDataType SPSoftwareDataType");
    if (HasCommand("ip")) {
        ParseIp(pairs);
    }
    else if (HasCommand("ifconfig")) {
        ParseIfConfig(pairs);
    }

    return !pairs.empty();
}

#endif
