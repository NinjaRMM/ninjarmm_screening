#if defined(__linux__)

#include "info.h"
#include "info_common.h"


// TODO: Check other linux family distributions
// zypper   suse            zypper search -i
// slackpkg slackware       ls /var/log/packages
// pacman   arch            pacman -Q
//--------------------------------------
bool
QueryOsPatches(vecStr &strings) {
    bool result = false;

    if (HasCommand("apt")) {
        result = GetCommandLineOutput("apt list --installed", strings);
    }
    else if (HasCommand("dnf")) {
        result = GetCommandLineOutput("dnf list installed", strings);
    }
    else if (HasCommand("yum")) {
        result = GetCommandLineOutput("yum list installed", strings);
    }

    if (result) {
        for (auto &str : strings) {
            RemoveExtraSpaces(Trim(str, " \n\r"));
        }
        return true;
    }
    return false;
}

//--------------------------------------
static void
GetInfo(vecStrStr &pairs, const std::string &key, const std::string &commandLine) {
    vecStr strings;

    if (GetCommandLineOutput(commandLine, strings)) {
        if (strings.size() >= 1) {
            pairs.emplace_back(key, Trim(strings[0], "\" \t\n\r"));
        }
    }
}

//--------------------------------------
static void
GetInfoSplit(vecStrStr &pairs, const std::string &key, const std::string &commandLine) {
    vecStr strings;

    if (GetCommandLineOutput(commandLine, strings)) {
        if (strings.size() >= 1) {
            vecStr parts = Split(strings[0], ':');
            if (parts.size() >= 2) {
                pairs.emplace_back(key, Trim(parts[1], "\" \t\n\r"));
            }
        }
    }
}

//--------------------------------------
void
ParseOsRelease(vecStrStr &pairs) {
    mapStrStr map;
    ParseKeyValueFile("/etc/os-release", map);

    auto it = map.find("PRETTY_NAME");
    if(it != map.end()) {
        pairs.emplace_back(std::string("OS Name"),  it->second);
    }

    it = map.find("VERSION");
    if(it != map.end()) {
        pairs.emplace_back(std::string("OS Version"),  it->second);
    }

    it = map.find("NAME");
    if(it != map.end()) {
        pairs.emplace_back(std::string("OS Manufacturer"),  it->second);
    }

    it = map.find("ID_LIKE");
    if(it != map.end()) {
        pairs.emplace_back(std::string("OS Family"),  it->second);
    }
}

//--------------------------------------
bool
QuerySystemInfo(vecStrStr &pairs) {
    // Maybe I could combine some of these system calls into ugly functions. But performance is not an issue here.
    GetInfo     (pairs, "Host Name", "hostname");

    if(HasFile("/etc/os-release")) {
        ParseOsRelease(pairs);
    }
    else if (HasCommand("lsb_release")) {
        GetInfoSplit(pairs, "OS Name",         "lsb_release -d");   //
        GetInfoSplit(pairs, "OS Version",      "lsb_release -r");   //
        GetInfoSplit(pairs, "OS Manufacturer", "lsb_release -i");   //
    }

    GetInfo     (pairs, "Kernel",                    "uname -mrs");
    GetInfoSplit(pairs, "Processor",                 "grep 'model name' /proc/cpuinfo | uniq");
    GetInfoSplit(pairs, "Total Physical Memory",     "grep MemTotal /proc/meminfo");      //
    GetInfoSplit(pairs, "Available Physical Memory", "grep MemAvailable /proc/meminfo");  //
    GetInfoSplit(pairs, "Virtual Memory: Max Size",  "grep VmallocTotal /proc/meminfo");  //
    GetInfoSplit(pairs, "Virtual Memory: Available", "grep VmallocUsed /proc/meminfo");   //

    if (HasCommand("ip")) {
        ParseIp(pairs);
    }
    else if (HasCommand("ifconfig")) {
        ParseIfConfig(pairs);
    }

    return !pairs.empty();
}

#endif
