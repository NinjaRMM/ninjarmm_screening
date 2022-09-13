#ifdef _WIN32
#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#endif
#ifdef __linux__
#include <sys/utsname.h>
#endif
#ifdef __APPLE__
#include <sys/utsname.h>
#endif
#include <iostream>
#include <string>

using namespace std;

class SysInfo
{
public:
    static string get_patch_info()
    {
#ifdef _WIN32
        HRESULT hres;
        string patch_ids = "";

        hres = CoInitializeEx(0, COINIT_MULTITHREADED);
        if (FAILED(hres))
        {
            cout << "Failed to initialize COM library. Error code = 0x"
                 << hex << hres << endl;
            return 1; // Program has failed.
        }
        hres = CoInitializeSecurity(
            NULL,
            -1,                          // COM authentication
            NULL,                        // Authentication services
            NULL,                        // Reserved
            RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
            RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
            NULL,                        // Authentication info
            EOAC_NONE,                   // Additional capabilities
            NULL                         // Reserved
        );

        if (FAILED(hres))
        {
            cout << "Failed to initialize security. Error code = 0x"
                 << hex << hres << endl;
            CoUninitialize();
            return 1; // Program has failed.
        }
        IWbemLocator *pLoc = NULL;
        hres = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator, (LPVOID *)&pLoc);

        if (FAILED(hres))
        {
            cout << "Failed to create IWbemLocator object."
                 << " Err code = 0x"
                 << hex << hres << endl;
            CoUninitialize();
            return 1; // Program has failed.
        }
        IWbemServices *pSvc = NULL;
        hres = pLoc->ConnectServer(
            _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
            NULL,                    // User name. NULL = current user
            NULL,                    // User password. NULL = current
            0,                       // Locale. NULL indicates current
            NULL,                    // Security flags.
            0,                       // Authority (for example, Kerberos)
            0,                       // Context object
            &pSvc                    // pointer to IWbemServices proxy
        );
        if (FAILED(hres))
        {
            cout << "Could not connect. Error code = 0x"
                 << hex << hres << endl;
            pLoc->Release();
            CoUninitialize();
            return 1; // Program has failed.
        }
        cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;
        hres = CoSetProxyBlanket(
            pSvc,                        // Indicates the proxy to set
            RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
            RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
            NULL,                        // Server principal name
            RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
            RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
            NULL,                        // client identity
            EOAC_NONE                    // proxy capabilities
        );
        if (FAILED(hres))
        {
            cout << "Could not set proxy blanket. Error code = 0x"
                 << hex << hres << endl;
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return 1; // Program has failed.
        }
        IEnumWbemClassObject *pEnumerator = NULL;
        hres = pSvc->ExecQuery(
            bstr_t("WQL"),
            bstr_t("SELECT * FROM Win32_QuickFixEngineerinug"),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator);
        if (FAILED(hres))
        {
            cout << "Query for QFE data failed."
                 << " Error code = 0x"
                 << hex << hres << endl;
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return 1; // Program has failed.
        }
        IWbemClassObject *pclsObj = NULL;
        ULONG uReturn = 0;
        while (pEnumerator)
        {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
            if (0 == uReturn)
            {
                break;
            }
            VARIANT vtProp;
            VariantInit(&vtProp);
            hr = pclsObj->Get(L"HotFixID", 0, &vtProp, 0, 0);
            wcout << "HotFixID : " << vtProp.bstrVal << endl;
            patch_ids.append(vtProp.bstrVal);
            patch_ids.append(",");
            VariantClear(&vtProp);
            pclsObj->Release();
        }
        pSvc->Release();
        pLoc->Release();
        pEnumerator->Release();
        CoUninitialize();
        return patch_ids;
#elif __linux__ || __APPLE__
        struct utsname buffer;
        if (uname(&buffer) == -1)
        {
            cout << "uname failed with error " << errno << endl;
            return nullptr;
        }
        cout << "SysName: " << buffer.sysname << endl;
        cout << "Machine: " << buffer.machine << endl;
        cout << "Nodename: " << buffer.nodename << endl;
        cout << "Release: " << buffer.release << endl;
        cout << "Version: " << buffer.version << endl;
        return string(buffer.release);
#else
        cout << "Unknown OS" << endl;
        return nullptr;
#endif
    }
};

/*
 * Could not build and test on Windows platform for lack of availability.
 * Used g++ on Linux platform. Tested on Linux and fairly confident the code would work on MacOS.
 */
int main()
{
    string patch_str = SysInfo::get_patch_info();
    cout << "Patches: " << patch_str << endl;
    return 0;
}
