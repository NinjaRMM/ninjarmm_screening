/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

// <-- Visual Studio must be installed to build this code -->

// <-- Included Windows headers for the code to compile -->

// Windows Headers
#include <windows.h>
#include <iwscapi.h>
#include <wscapi.h>

// <-- Included Standard Library headers for the code to compile -->

// Standard Library Headers
#include <iostream>
#include <map>
#include <string>

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;

    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;

    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

    CoInitialize(nullptr); // <-- Must initialize the COM library on the current thread before calling CoCreateInstance -->

    // <-- Better use 'nullptr' instead 'NULL' -->
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        PtrProductList->Release(); // <-- Must release interface before returning -->
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        PtrProductList->Release(); // <-- Must release interface before returning -->
        std::cout << "Failed to query product count.";
        return false;
    }

    // Treating warnings as errors. Code won't compile
    // for (uint32_t i = 0; i < ProductCount; i++) <-- Mixing unsigned and signed types

    for (int32_t i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            std::cout << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product state.";
            continue;
        }

        if (ProductState == WSC_SECURITY_PRODUCT_STATE_ON)
        {
            state = L"On";
        }
        else if (ProductState == WSC_SECURITY_PRODUCT_STATE_OFF)
        {
            state = L"Off";
        }
        else
        {
            state = L"Expired";
        }

        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    PtrProductList->Release(); // <-- Must release interface before returning -->

    if (thirdPartyAVSoftwareMap.size() == 0) // <-- Better use 'empty()' instead of comparing with 'size() == 0' -->
    {
        std::cout << "No AV software found.";
        return false;
    }

    CoUninitialize();// <-- Must uninitialize the COM library on the current thread before returning -->
    return true;
}

// <-- Created main entry point to execute the code -->

auto main() -> int
{
    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap;
    queryWindowsForAVSoftwareDataWSC(thirdPartyAVSoftwareMap);
    return 0;
}