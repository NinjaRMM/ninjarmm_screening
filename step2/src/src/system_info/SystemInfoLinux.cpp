#if defined(linux)

#include "SystemInfo.hpp"
#include <fstream>
#include <regex>


static std::string getFileValueByRegex(const std::string& file, const std::regex& pattern) {
    std::ifstream inputFile(file);
    std::string line;
    std::smatch match;
    while (std::getline(inputFile, line)) {
        if (std::regex_match(line, match, pattern)) {
            return match.str(1);
        }
    }
    return {};
}

std::string SystemInfo::platform() const {
    return getFileValueByRegex("/etc/os-release", std::regex{"PRETTY_NAME=\"(.*)\""});
}

std::string SystemInfo::cpu() const {
    return getFileValueByRegex("/proc/cpuinfo", std::regex{"^model name\\s*:\\s*(.*)"});
}

std::string SystemInfo::memory() const {
    int memory{};
    auto value = getFileValueByRegex("/proc/meminfo", std::regex{"^MemTotal:\\s*(.*) .*"});

    try {
        memory = std::stoi(value);
    } catch (const std::exception&) {
        return {};
    }
    return std::string{std::to_string(memory / 1024 / 1024) + " GB"};
}

#endif
