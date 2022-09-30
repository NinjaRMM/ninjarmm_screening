 
#include <iostream>
#define _WIN32_DCOM
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

using namespace std;

std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen)
{
    int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);
      std::string dblstr(len, '\0');
    len = ::WideCharToMultiByte(CP_ACP, 0 ,pstr, wslen ,&dblstr[0], len,NULL, NULL);
    return dblstr;
}

std::string ConvertBSTRToMBS(BSTR bstr)
{
    int wslen = ::SysStringLen(bstr);
    return ConvertWCSToMBS((wchar_t*)bstr, wslen);
}
int main()
{
    

    string RESULT; // CPU Manufacturer 



    HRESULT hres;
     
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        return 1;             
    }

    hres = CoInitializeSecurity(NULL,-1,   
        NULL,                         
        NULL,                        
        RPC_C_AUTHN_LEVEL_DEFAULT,     
        RPC_C_IMP_LEVEL_IMPERSONATE,    
        NULL,                         
        EOAC_NONE,                    
        NULL                          
    );

    if (FAILED(hres))
    {
        CoUninitialize();
        return 1;                   
    }
    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(CLSID_WbemLocator,0,CLSCTX_INPROC_SERVER,IID_IWbemLocator, (LPVOID*)&pLoc);
    if (FAILED(hres))
    {
        CoUninitialize();
        return 1;                
    }
 
    IWbemServices* pSvc = NULL;
 
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
       
        pLoc->Release();
        CoUninitialize();
        return 1;                // Program has failed.
    }

  
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
        
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;             
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
       // ok    bstr_t("WQL"), bstr_t("SELECT Manufacturer FROM Win32_ComputerSystem"),
        bstr_t("WQL"), bstr_t("SELECT * FROM Win32_BIOS"),
        


        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;             
    }
     IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;
     while (pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn)== WBEM_S_NO_ERROR)
     {
        if (0 == uReturn){break;}

        VARIANT vtProp1;
        HRESULT hr = pclsObj->Get(L"BuildNumber", 0, &vtProp1, 0, 0);
          RESULT = ConvertBSTRToMBS(vtProp1.bstrVal);
        cout << RESULT  << endl;
         
        VariantClear(&vtProp1);

        VARIANT vtProp2;
          hr = pclsObj->Get(L"Caption", 0, &vtProp2, 0, 0);
        RESULT = ConvertBSTRToMBS(vtProp2.bstrVal);
        cout << RESULT << endl;




        VariantClear(&vtProp2);

        pclsObj->Release();
    }
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
    return 0;   
 }
 
 