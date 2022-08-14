#include <iostream>
#include <map>
#include <stdio.h>
#include <atlbase.h>
#include <atlstr.h>
#include <wscapi.h>
#include <iwscapi.h>

/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

Compiled and tested on VC++

*/


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


    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        // WILL - Release List - Created successfully above
        PtrProductList->Release();

        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        // WILL - Release List - Created successfully above
        PtrProductList->Release();

        std::cout << "Failed to query product count.";
        return false;
    }

    for (LONG i = 0; i < ProductCount; i++)
    {
        // WILL - Continue Execution Cleanup
        if (nullptr != PtrVal)
        {
            SysFreeString(PtrVal);
            PtrVal = nullptr;
        }

        if (nullptr != PtrProduct)
        {
            PtrProduct->Release();
            PtrProduct = nullptr;
        }
 
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
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

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
    }

    // WILL - Added post loop cleanup
    if (nullptr != PtrVal)
    {
        SysFreeString(PtrVal);
        PtrVal = nullptr;
    }

    if (nullptr != PtrProduct)
    {
        PtrProduct->Release();
        PtrProduct = nullptr;
    }
    
    if (nullptr != PtrProductList)
    {
        PtrProductList->Release();
        PtrProductList = nullptr;
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}

int main()
{
    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap;

    CoInitializeEx(0, COINIT_APARTMENTTHREADED);

    if (queryWindowsForAVSoftwareDataWSC(thirdPartyAVSoftwareMap))
    {
        for (auto const& x : thirdPartyAVSoftwareMap)
        {
            std::wcout << x.first << " : "
                << x.second.Name << " : "
                << x.second.Description << " : "
                << x.second.DefinitionUpdateTime << " : "
                << std::wstring( x.second.DefinitionStatus.begin(), x.second.DefinitionStatus.end() ) << " : "
                << x.second.Version << " : "
                << x.second.ProductState
                << std::endl;
        }
    }
    else  
        std::cout << std::endl << "No Third Party AV Software Found!" << std::endl;

    CoUninitialize();

    return 0;
}
