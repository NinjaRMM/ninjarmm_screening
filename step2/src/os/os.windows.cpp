#include "os.hpp"
#include <wuapi.h>
#include <iostream>
#include <ATLComTime.h>
#include <wuerror.h>
#include <iostream>


/*
* This File Implements os.hpp for the windows OS
*/


// BSTRtoUTF8 copied from https://stackoverflow.com/questions/52000140/bstr-conversion-to-utf-8
// convert wstring to UTF-8 string
std::string BSTRtoUTF8(BSTR bstr) {
    int len = SysStringLen(bstr);
    // special case because a NULL BSTR is a valid zero-length BSTR,
    // but regular string functions would balk on it
    if(len == 0) return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, bstr, len, NULL, 0, NULL, NULL);
    std::string ret(size_needed, '\0');
    WideCharToMultiByte(CP_UTF8, 0, bstr, len, ret.data(), ret.size(), NULL, NULL);
    return ret;
}


std::vector<OsPatch> getPatches(){
    //Code from https://www.appsloveworld.com/cplus/100/124/windows-update-agent-pure-win32-apis
    HRESULT hr;
    hr = CoInitialize(NULL);

    IUpdateSession* iUpdate;
    IUpdateSearcher* searcher;
    ISearchResult* results;
    BSTR criteria = SysAllocString(L"IsInstalled=1 or IsHidden=1 or IsPresent=1");

    hr = CoCreateInstance(CLSID_UpdateSession, NULL, CLSCTX_INPROC_SERVER, IID_IUpdateSession, (LPVOID*)&iUpdate);
    hr = iUpdate->CreateUpdateSearcher(&searcher);

    //std::wcout << L"Searching for updates ..." <<  std::endl;
    hr = searcher->Search(criteria, &results);
    SysFreeString(criteria);

    switch (hr)
    {
    case S_OK:
        break;
    case WU_E_LEGACYSERVER:
        std::wcout << L"No server selection enabled" <<  std::endl;
        return {};
    case WU_E_INVALID_CRITERIA:
        std::wcout << L"Invalid search criteria" <<  std::endl;
        return {};
    default:
        return {};
    }

    IUpdateCollection* updateList;
    IUpdate* updateItem;
    LONG updateSize;
    BSTR updateName;
    DATE retdate;

    results->get_Updates(&updateList);
    updateList->get_Count(&updateSize);

    if (updateSize == 0) return {};

    std::vector<OsPatch> patches;
    for (LONG i = 0; i < updateSize; i++)
    {
        updateList->get_Item(i, &updateItem);
        updateItem->get_Title(&updateName);
        updateItem->get_LastDeploymentChangeTime(&retdate);
        COleDateTime odt;
        odt.m_dt = retdate;

        patches.push_back(OsPatch{ 
            .name = BSTRtoUTF8(updateName),
            .date = (LPCTSTR)odt.Format(_T("%A, %B %d, %Y"))
        });
    }
    ::CoUninitialize();
    return patches;
}


std::string getComputerName() {

    constexpr long INFO_BUFFER_SIZE = 32767;
    static bool setted = false;
    static TCHAR  infoBuf[INFO_BUFFER_SIZE];

    if (setted) return { infoBuf , infoBuf + INFO_BUFFER_SIZE };

    DWORD  bufCharCount = INFO_BUFFER_SIZE;

    // Get and display the name of the computer.
    GetComputerName(infoBuf, &bufCharCount);
    setted = true;
    
    return { infoBuf , infoBuf + INFO_BUFFER_SIZE };
}


long getRamMB() {
    MEMORYSTATUSEX statex;

    statex.dwLength = sizeof(statex); // I misunderstand that

    GlobalMemoryStatusEx(&statex); 
    return statex.ullTotalPhys / 1024 / 1024;
}


CString GetStringFromReg(HKEY keyParent, CString keyName, CString keyValName)
{
    CRegKey key;
    CString out;
    if (key.Open(keyParent, keyName, KEY_READ) == ERROR_SUCCESS)
    {
        ULONG len = 256;
        key.QueryStringValue(keyValName, out.GetBuffer(256), &len);
        out.ReleaseBuffer();
        key.Close();
    }
    return out;
}

std::string getOsName() {
    return (LPCTSTR)GetStringFromReg(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"ProductName");
}
