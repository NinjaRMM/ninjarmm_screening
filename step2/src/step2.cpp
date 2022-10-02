/// This solution was written in C++17.
/// Tested on platforms Windows and Linux.
/// I did not support the solution for Mac as I don't have an environment for dev and testing for Mac.
/// But the code is prepared for adding support for it.
///
/// The solution approach is as follows:
/// Created an abstract interface SystemInfo for retrieving system info and installed patches from OS.
/// The interface is generic for all OSes, and each OS implements the interface in its subclass.
///
/// The system info is parsed and stored in a map to allow for more flexibility in adding multiple
/// types of informations, some may be available in a OS and others may not.
///
/// The patches are handled as a list. Since it's very OS specific there isn't much parsing on it.
/// The user of the interface is responsible for checking which patches are required or not based on the platform.
///

#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <cstring>
#include <optional>
#include <map>
#include <unordered_map>
#include <cctype>
#include <memory>
#include <cassert>
#include <vector>
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include "sdkddkver.h"
#include <iostream>
#include <ShlObj.h>
#include <atlbase.h>
#endif // defined(_WIN32) || defined(_WIN64)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines

#define LOG_CERR \
    std::cerr << __FILE__ << ':' << __LINE__ << ": "


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Utils

/// Read file contents to a string
auto ReadFileToString(const std::string &filename) -> std::optional<std::string>
{
    std::string string;
    std::fstream fstream(filename, std::ios::in | std::ios::binary);
    if (!fstream) {
        LOG_CERR << std::strerror(errno) << ' ' << filename << '\n';
        return std::nullopt;
    }
    string.reserve(fstream.tellg());
    fstream.seekg(0, std::ios::beg);
    string.assign((std::istreambuf_iterator<char>(fstream)), std::istreambuf_iterator<char>());
    return string;
}

/// Parse key=value lines in string
auto ParseKeyValues(const std::string &string)
{
    // map of key-value pairs
    std::map<std::string, std::string> map;
    // aux vars for parsing
    bool is_key = true;
    std::string key, val;
    // loop through chars and parse key=values
    for (char ch : string) {
        if (ch == '=') is_key = false;
        else if (ch == '"') continue;
        else if (ch == '\n' || ch == '\r') {
            is_key = true;
            if (!key.empty()) map[key] = val;
            key.clear();
            val.clear();
        }
        else if (std::isprint(ch)) {
            (is_key ? key : val) += ch;
        }
    }
    return map;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// SystemInfo

/// Abstract class for retrieving information of any OS
class SystemInfo {
  public:
    SystemInfo() = default;
    virtual ~SystemInfo() = default;
    
    /// Build instance of SystemInfo based on platform
    static auto Build() -> std::unique_ptr<SystemInfo>;

    /// Load information from the underlying system
    virtual bool Load() = 0;

    /// Supported Info
    enum class Key {
        Id,
        Name,
        Version,
        NumProc,
        ProcType,
        MemTotal,
    };

    /// Retrieve any system info by key
    [[nodiscard]] virtual auto GetInfo(Key key) const -> std::string_view = 0;

    /// Retrieve list of install OS patches
    [[nodiscard]] virtual auto GetInstalledPatches() const -> const std::vector<std::wstring>& = 0;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Linux

#ifdef __linux__
/// Implement SystemInfo interface for Linux
class SystemInfoLinux final : public SystemInfo {
  public:
    /// Load information from the underlying system
    bool Load() final {
        auto os_release = ReadFileToString("/etc/os-release");
        if (!os_release) {
            LOG_CERR << "Failed to retrieve Linux OS release" << '\n';
            return false;
        }
        info_map = ParseKeyValues(*os_release);
        auto os_version = ReadFileToString("/proc/version");
        if (!os_version) {
            LOG_CERR << "Failed to retrieve Linux OS version" << '\n';
            return false;
        }
        info_map["VERSION"] = std::move(*os_version);
        // ToDo: retrieve Memory and CPU info from /proc/meminfo and /proc/cpuinfo

        return LoadPatchList();
    }

    /// Retrieve any system info by key
    [[nodiscard]] auto GetInfo(Key key) const -> std::string_view final {
        const char* key_str = nullptr;
        switch (key) {
            case Key::Id: key_str = "ID"; break;
            case Key::Name: key_str = "NAME"; break;
            case Key::Version: key_str = "VERSION"; break;
            case Key::NumProc: break;
            case Key::ProcType: break;
            case Key::MemTotal: break;
        }
        if (key_str)
            if (auto it = info_map.find(key_str); it != info_map.end())
                return it->second;
        return "";
    }

    /// Retrieve list of install OS patches
    [[nodiscard]] auto GetInstalledPatches() const -> const std::vector<std::wstring>& final {
        return patches;
    }

  private:
    /// Run command on linux and get output to stdout
    auto RunCmd(const char* cmd) -> std::optional<std::string> {
        char buffer[BUFSIZ];
        std::ostringstream result;
        FILE *pipe = popen(cmd, "r");
        if (!pipe) {
            LOG_CERR << "Failed running command: " << cmd << '\n';
            return std::nullopt;
        }
        while (fgets(buffer, sizeof(buffer), pipe) != NULL)
            result << buffer;
        pclose(pipe);
        return result.str();
    }

    /// Load installed patches
    /// Now, linux does not follow the concept of downloadable OS patches like windows
    /// So instead we're going to show the installed packages which we can use to query for libs, dependencies, drivers and etc.
    bool LoadPatchList() {
        std::optional<std::string> list;
        if (info_map["ID"] == "arch" || info_map["ID_LIKE"] == "arch")
            list = RunCmd("pacman -Q");
        else if (info_map["ID"] == "debian" || info_map["ID_LIKE"] == "debian")
            list = RunCmd("apt list --installed");
        // ToDo: add support to other linux distributions

        if (!list)
            return false;
        
        std::stringstream ss(*list);
        std::string str;
        while (std::getline(ss, str, '\n'))
            patches.emplace_back(std::wstring(std::move_iterator(str.begin()), std::move_iterator(str.end())));

        return true;
    }

  private:
    /// Map of key-values of OS info
    std::map<std::string, std::string> info_map;
    /// List of install OS patches
    std::vector<std::wstring> patches;
};
#endif // __linux__


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Windows

#if defined(_WIN32) || defined(_WIN64)
/// Implement SystemInfo interface for Windows
class SystemInfoWindows final : public SystemInfo {
  public:
    /// Load information from the underlying system
    bool Load() final {
        SYSTEM_INFO info{};
        GetSystemInfo(&info);
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof (statex);
        GlobalMemoryStatusEx (&statex);
        info_map[Key::Name] = "Windows";
        info_map[Key::NumProc] = std::to_string(info.dwNumberOfProcessors);
        info_map[Key::ProcType] = std::to_string(info.dwProcessorType);
        info_map[Key::MemTotal] = std::to_string(statex.ullTotalPhys / 1024 / 1024) + " MB";
        // ToDo: retrieve more information, windows version, etc, disk usage, etc.
        LoadWindowsUpdatePatches();
        return true;
    }

    /// Retrieve any system info by key
    [[nodiscard]] auto GetInfo(Key key) const -> std::string_view final {
        if (auto it = info_map.find(key); it != info_map.end())
            return it->second;
        return "";
    }

    /// Retrieve list of install OS patches
    [[nodiscard]] auto GetInstalledPatches() const -> const std::vector<std::wstring>& final {
        return patches;
    }

  private:
    /// Load Installed Windows Updates
    void LoadWindowsUpdatePatches() {
        HRESULT hr = CoInitialize(NULL);
        if (SUCCEEDED(hr))
        {
            CComPtr<IShellItem> pUpdates;
            CComPtr<IEnumShellItems> pShellEnum;

            hr = SHGetKnownFolderItem(FOLDERID_AppUpdates, static_cast<KNOWN_FOLDER_FLAG>(0), nullptr, IID_PPV_ARGS(&pUpdates));
            hr = pUpdates->BindToHandler(nullptr, BHID_EnumItems, IID_PPV_ARGS(&pShellEnum));
            if (pShellEnum)
            {
                do {
                    CComPtr<IShellItem> pItem;
                    CComHeapPtr<WCHAR> szName;

                    hr = pShellEnum->Next(1, &pItem, nullptr);
                    if (pItem)
                    {
                        HRESULT hres = pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &szName);
                        patches.emplace_back(std::wstring(szName));
                    }
                } while (hr == S_OK);
            }
        }
        CoUninitialize();
    }

  private:
    /// Map of key-values of OS info
    std::unordered_map<Key, std::string> info_map;
    /// List of install OS patches
    std::vector<std::wstring> patches;
};
#endif // defined(_WIN32) || defined(_WIN64)


////////////////////////////////////////////////////////////////////////////////////////////////////
/// TODO: Mac

#ifdef __APPLE__
/// Implement SystemInfo interface for Mac
class SystemInfoMac final : public SystemInfo {
  public:
    /// Load information from the underlying system
    bool Load() final {
        return false;
    }

    /// Retrieve any system info by key
    [[nodiscard]] auto GetInfo(Key key) const -> std::string_view final {
        throw std::runtime_error("system info not found");
    }

    /// Retrieve list of install OS patches
    [[nodiscard]] auto GetInstalledPatches() const -> const std::vector<std::wstring>& final {
        throw std::runtime_error("unimplemented");
    }
};
#endif // __APPLE__


////////////////////////////////////////////////////////////////////////////////////////////////////
/// SystemInfo Details

auto SystemInfo::Build() -> std::unique_ptr<SystemInfo> {
#ifdef __linux__
    return std::make_unique<SystemInfoLinux>();
#elif defined(_WIN32) || defined(_WIN64)
    return std::make_unique<SystemInfoWindows>();
#elif __APPLE__
    return std::make_unique<SystemInfoMac>();
#else
#error "Unsupported operating system"
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Main

/// See file top comment for more info
int main()
{
    auto sys_info = SystemInfo::Build();
    assert(sys_info->Load());

    // Display desired system info
    std::cout << "OS ID: " << sys_info->GetInfo(SystemInfo::Key::Id) << '\n'
              << "OS Name: " << sys_info->GetInfo(SystemInfo::Key::Name) << '\n'
              << "OS Version: " << sys_info->GetInfo(SystemInfo::Key::Version) << '\n'
              << "Memory total: " << sys_info->GetInfo(SystemInfo::Key::MemTotal) << '\n'
              << "Processor Type: " << sys_info->GetInfo(SystemInfo::Key::ProcType) << '\n'
              << "Num of Processors: " << sys_info->GetInfo(SystemInfo::Key::NumProc) << std::endl;

    // Display list of OS patches
    std::cout << "\nInstall OS Patches:\n";
    for (const auto& patch : sys_info->GetInstalledPatches()) {
        std::wcout << patch << std::endl;
    }

    return 0;
}
