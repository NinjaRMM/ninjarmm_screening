/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

// PR-COMMENT: The following includes were missing

#include <iostream>
#include <map>
#include <string>

#include "iwscapi.h"
#include "windows.h"
#include "wscapi.h"

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

// PR-COMMENT: The return value of this API might lead to a confusion. If there are no AV registered, the
// API will return false, same value when a call to a Windows API fails. Is this intended?
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

    // PR-COMMENT: This might fail if the class does not exist in the registry or
    // the COM library is not initialized in the current thread.
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        // PR-COMMENT: Missing new line in error messages. If multiple calls fail
        // in the loop below error messages will be outputted in the same line.
        std::cout << "Failed to create WSCProductList object. " << std::endl;
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. " << std::endl;

        // PR-COMMENT: Product list should be released at this point.
        PtrProductList->Release();
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count." << std::endl;

        // PR-COMMENT: Product list should be released at this point.
        PtrProductList->Release();
        return false;
    }

    // PR-COMMENT: Not critical, but size_t could be a better fit for loop iteration (even
    // if using unsigned as in this case).
    for (uint32_t i = 0; i < ProductCount; i++)
    {
        // PR-COMMENT: The issue with this approach is, in my opinion, that the caller of this API
        // will not be aware of any error at this point so if there is any error, it won't get the
        // full list.

        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product." << std::endl;
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            std::cout << "Failed to query AV product name." << std::endl;
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));
        // PR-COMMENT: Missing resource release.
        SysFreeString(PtrVal);

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            // PR-COMMENT: Missing resource release.
            PtrProduct->Release();
            std::cout << "Failed to query AV product state." << std::endl;
            continue;
        }

        // PR-COMMENT: Not critical, but probably a switch is more readable.
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
            // PR-COMMENT: Missing resource release.
            PtrProduct->Release();
            std::cout << "Failed to query AV product definition state." << std::endl;
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            // PR-COMMENT: Missing resource release.
            PtrProduct->Release();
            std::cout << "Failed to query AV product definition state." << std::endl;
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

    // PR-COMMENT: Product list should be released at this point.
    PtrProductList->Release();

    // PR-COMMENT: See API comment above. Based on that, this might not be required.
    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}

int main()
{
    HRESULT hr;
    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap;

    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        std::cout << "Failed to initialize.";
        return -1;
    }

    if (queryWindowsForAVSoftwareDataWSC(thirdPartyAVSoftwareMap))
    {
        for (const auto& i : thirdPartyAVSoftwareMap)
        {
            std::wcout << i.first << std::endl;
        }
    }
    else
    {
        std::cout << "Failed to query with error: " << GetLastError() << std::endl;
        return -1;
    }

    CoUninitialize();

    return 0;
}
