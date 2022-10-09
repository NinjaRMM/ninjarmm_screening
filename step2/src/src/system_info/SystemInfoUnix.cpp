#if defined(__APPLE__) || defined(linux)

#include "SystemInfo.hpp"

#include <sys/statvfs.h>


std::string SystemInfo::disk() const {
    struct statvfs stat {};
    if (statvfs(".", &stat) != 0) {
        return {};
    }

    auto dSize = (static_cast<uint64_t>(stat.f_blocks) * stat.f_frsize) / 1024 / 1024 / 1024;
    return std::string{std::to_string(dSize) + " GB"};
}

#endif
