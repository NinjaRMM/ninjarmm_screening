#include <memory>
#include <math.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

//Linux systems
#ifdef linux
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>

//Windows systems
#elif defined _WIN32  || defined _WIN64
#include <unordered_map>
#include <cassert>
#ifndef _WIN32_DCOM
#   define _WIN32_DCOM
#endif
#ifndef UNICODE
#   define UNICODE
#endif
#include <Windows.h>
#include <comdef.h>
#include <comutil.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

_COM_SMARTPTR_TYPEDEF(IWbemLocator, __uuidof(IWbemLocator));
_COM_SMARTPTR_TYPEDEF(IWbemServices, __uuidof(IWbemServices));
_COM_SMARTPTR_TYPEDEF(IEnumWbemClassObject, __uuidof(IEnumWbemClassObject));
_COM_SMARTPTR_TYPEDEF(IWbemClassObject, __uuidof(IWbemClassObject));

inline void TESTHR(HRESULT hr) {
    if (FAILED(hr)) {
        _com_issue_error(hr);
    }
}

//Unsupported systems
#else
#error "Unsupported system"
#endif

constexpr uint64_t BYTES_PER_KB = 1024;
constexpr uint64_t BYTES_PER_MB = BYTES_PER_KB * 1024;

class Hal {
public:
    static std::shared_ptr<Hal> get_hal(); /* defined by hal implementation */
    virtual ~Hal() = default;
    virtual std::string os_name() const = 0;
    virtual std::string os_release() const = 0;
    virtual std::string os_version() const = 0;
    virtual void print_installed_packages() = 0;
    virtual std::string architecture() const = 0;
    virtual uint64_t total_ram() const = 0;
    virtual uint64_t free_ram() = 0;
    virtual std::string user_name() const = 0;
};

class OsInfo {
public:
    OsInfo(std::shared_ptr<Hal>& hal) : m_hal(hal) {
    }

    std::string name() const {
        return m_hal.get()->os_name();
    };

    std::string release() const {
        return m_hal.get()->os_release();
    };

    std::string version() const {
        return m_hal.get()->os_version();
    };

    void print_installed_packages() {
        m_hal.get()->print_installed_packages();
    };

    friend std::ostream& operator<<(std::ostream& cout, const OsInfo& os_info) {
        std::ostringstream ss;
        ss <<
            "OS:             " << os_info.name() << std::endl <<
            "Release:        " << os_info.release() << std::endl <<
            "Version:        " << os_info.version() << std::endl;
        return cout << ss.str();
    }
private:
    std::shared_ptr<Hal> m_hal;
};

class HardwareInfo {
public:
    HardwareInfo(std::shared_ptr<Hal>& hal) : m_hal(hal) {
    }

    std::string architecture() const {
        return m_hal.get()->architecture();
    };
    uint64_t total_ram_mb() const {
        return floor(m_hal.get()->total_ram() / BYTES_PER_MB);
    };
    uint64_t free_ram_mb() const {
        return floor(m_hal.get()->free_ram() / BYTES_PER_MB);
    };
    friend std::ostream& operator<<(std::ostream& cout, const HardwareInfo& hw_info) {
        std::ostringstream ss;
        ss <<
            "Architecture:   " << hw_info.architecture() << std::endl <<
            "Total RAM (MB): " << hw_info.total_ram_mb() << std::endl <<
            "Free RAM (MB):  " << hw_info.free_ram_mb() << std::endl;
        return cout << ss.str();
    }
private:
    std::shared_ptr<Hal> m_hal;
};

class UserInfo {
public:
    UserInfo(std::shared_ptr<Hal>& hal) : m_hal(hal) {
    }

    std::string user_name() const {
        return m_hal.get()->user_name();
    };
    friend std::ostream& operator<<(std::ostream& cout, const UserInfo& user_info) {
        std::ostringstream ss;
        ss <<
            "Username:       " << user_info.user_name() << std::endl;
        return cout << ss.str();
    }
private:
    std::shared_ptr<Hal> m_hal;
};

//Linux systems
#ifdef linux

class LinuxHal : public Hal {
public:
    LinuxHal();
    std::string os_name() const override;
    std::string os_release() const override;
    std::string os_version() const override;
    void print_installed_packages() override;
    std::string architecture() const override;
    uint64_t total_ram() const override;
    uint64_t free_ram() override;
    std::string user_name() const override;

private:
    struct utsname m_utsname;
    struct sysinfo m_sysinfo;
};

std::shared_ptr<Hal> Hal::get_hal() {
    return std::shared_ptr<Hal>(new LinuxHal());
}

LinuxHal::LinuxHal() {
    uname(&m_utsname);
    sysinfo(&m_sysinfo);
}

std::string LinuxHal::os_name() const {
    return std::string(m_utsname.sysname);
}

std::string LinuxHal::os_release() const {
    return std::string(m_utsname.release);
}

std::string LinuxHal::os_version() const {
    return std::string(m_utsname.version);
}

std::string LinuxHal::architecture() const {
    return std::string(m_utsname.machine);
}

uint64_t LinuxHal::total_ram() const {
    return m_sysinfo.totalram;
}

uint64_t LinuxHal::free_ram() {
    sysinfo(&m_sysinfo);
    return m_sysinfo.freeram;
}

std::string LinuxHal::user_name() const {
    return std::string(getlogin());
}

void LinuxHal::print_installed_packages() {
    if (system("apt list --installed") != 0) {
        std:: cout << "Could not get the package list" << std::endl;
    }
}

//Windows systems
#elif defined _WIN32 || defined _WIN64

class WindowsHal : public Hal {
public:
    WindowsHal();
    ~WindowsHal();
    std::string os_name() const override;
    std::string os_release() const override;
    std::string os_version() const override;
    void print_installed_packages() override;
    std::string architecture() const override;
    uint64_t total_ram() const override;
    uint64_t free_ram() override;
    std::string user_name() const override;

private:
    MEMORYSTATUSEX statex;
    uint64_t m_total_ram;
    std::unordered_map<std::string, std::string> m_osCache;

    template <typename T>
    T get_property(IWbemClassObjectPtr& pOS, wchar_t* property_name);
};

std::shared_ptr<Hal> Hal::get_hal() {
    return std::shared_ptr<Hal>(new WindowsHal());
}

template <>
std::string WindowsHal::get_property(IWbemClassObjectPtr& pOS, wchar_t* property_name) {
    _variant_t prop_val = VT_EMPTY;
    CIMTYPE cim_type = 0;
    TESTHR(pOS->Get(property_name, 0, &prop_val, &cim_type, 0));
    assert(cim_type == 8);
    char* lpszText2 = _com_util::ConvertBSTRToString(static_cast<_bstr_t>(prop_val));
    if (lpszText2) {
        std::string ret(lpszText2);
        delete[] lpszText2;
        return ret;
    } else {
        return std::string();
    }
}

template <>
uint64_t WindowsHal::get_property(IWbemClassObjectPtr& pOS, wchar_t* property_name) {
    _variant_t prop_val;
    CIMTYPE cim_type;
    TESTHR(pOS->Get(property_name, 0, &prop_val, &cim_type, 0));
    assert(cim_type == 21);
    uint64_t ret = prop_val;
    return ret;
}

WindowsHal::WindowsHal() {
    CoInitialize(NULL);
    TESTHR(CoInitializeSecurity(0, -1, 0, 0, RPC_C_AUTHN_LEVEL_DEFAULT,
                                RPC_C_IMP_LEVEL_IMPERSONATE, 0, EOAC_NONE, 0));

    IWbemLocatorPtr pLoc = nullptr;
    TESTHR(CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
                            IID_IWbemLocator, (void**)&pLoc));

    IWbemServicesPtr pSvc = nullptr;
    TESTHR(pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"),
                               0, 0, 0, 0, 0, 0, &pSvc));

    TESTHR(CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, 0,
                             RPC_C_AUTHN_LEVEL_CALL,
                             RPC_C_IMP_LEVEL_IMPERSONATE, 0,EOAC_NONE));

    IEnumWbemClassObjectPtr pObjEnum;
    const long flags = WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY;
    TESTHR(pSvc->ExecQuery(_bstr_t("WQL"),
                            _bstr_t("SELECT * FROM Win32_OperatingSystem"),
                            flags, 0, &pObjEnum));

    ULONG returned = 0;
    IWbemClassObjectPtr pOS;
    TESTHR(pObjEnum->Next(WBEM_INFINITE, 1, &pOS, &returned));
    m_osCache.insert(std::make_pair(std::string("caption"), get_property<std::string>(pOS, L"Caption")));
    m_osCache.insert(std::make_pair(std::string("version"), get_property<std::string>(pOS, L"Version")));
    m_osCache.insert(std::make_pair(std::string("release"), get_property<std::string>(pOS, L"BuildNumber")));
    m_total_ram = get_property<uint64_t>(pOS, L"TotalVisibleMemorySize") * BYTES_PER_KB;

    pOS->Release();
    pOS = nullptr;
    pSvc->Release();
    pSvc = nullptr;
    pLoc->Release();
    pLoc = nullptr;
    pObjEnum->Release();
    pObjEnum = nullptr;

    CoUninitialize();
}

WindowsHal::~WindowsHal() {
}

std::string WindowsHal::os_name() const {
    return m_osCache.at("caption");
}

std::string WindowsHal::os_release() const {
    return m_osCache.at("release");
}

std::string WindowsHal::os_version() const {
    return m_osCache.at("version");
}

std::string WindowsHal::architecture() const {
    return os_name();
}

uint64_t WindowsHal::total_ram() const {
    return m_total_ram;
}

uint64_t WindowsHal::free_ram() {
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx (&statex);
    return statex.ullAvailPhys;
}

std::string WindowsHal::user_name() const {
    char* username = getenv("username");
    if (username != nullptr) {
        return std::string(username);
    } else {
        return std::string("");
    }

}

void WindowsHal::print_installed_packages() {
    system("wmic qfe list full");
}

#else

std::shared_ptr<Hal> Hal::get_hal() {
    throw (std::runtime_error("Unsupported OS"));
}

#endif


/*
    It was not compiled nor tested on Apple platform, thus this code doesn't support it. Also, as
    this code is expected to run across multiple platforms, it was made available a configure.ac and
    a Makefile.am in order to properly configure the compilation environment.
    The validation performed was manual validation once all information depends on which computer
    it's running on. Therefore, it was validated on local Linux and on online Windows
    (https://godbolt.org/ using compiler "x64 msvc v19.latest).
    This code relies on system() call with hardcoded application called, so it's not portable and
    it's not expected to be in production environment.
    It was checked against Valgrind on local Ubuntu Linux:
        ==2782885==
        ==2782885== HEAP SUMMARY:
        ==2782885==     in use at exit: 0 bytes in 0 blocks
        ==2782885==   total heap usage: 59 allocs, 59 frees, 89,054 bytes allocated
        ==2782885==
        ==2782885== All heap blocks were freed -- no leaks are possible
        ==2782885==
        ==2782885== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/
int main() {
    auto hal = Hal::get_hal();
    OsInfo os_info(hal);
    HardwareInfo hw_info(hal);
    UserInfo user_info(hal);

    std::ostringstream ss;
    ss <<
        "====== OS ======" << std::endl <<
        os_info << std::endl <<
        "=== Hardware ===" << std::endl <<
        hw_info << std::endl <<
        "===== User =====" << std::endl <<
        user_info << std::endl;

    std::cout << ss.str() << std::endl <<
        "=== Packages ===" << std::endl;
    os_info.print_installed_packages();
    return 0;
}
