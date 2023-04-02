/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

// not sure if this is part of the review
// I would expect all code set to be reviewed can be at least compiled without error
// so these includes should have been included somewhere, another header file if not here
#include<map>
#include<string>
#include<iostream>

#include<combaseapi.h>
#include<wscapi.h>
#include<iwscapi.h>
#include<Winerror.h>

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
    //BSTR PtrVal = nullptr;      // should avoid generic names, variables should be named for it's explicit purpose

    // std::wstring displayName, versionNumber, state, timestamp; // versionNumber is unused, move other variables closer to their usage

    // variable should be decalared close to where it's first used, this ensures in the future if the code is removed, declaration is removed as well.
    IWSCProductList* PtrProductList = nullptr;
    HRESULT hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr) || PtrProductList == nullptr) // Since I'm not sure what the function CoCreateInstance does, just to ensure PtrProductList is set properly before moving on
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    LONG ProductCount = 0; // variable should be decalared close to where it's first used
    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }

    IWscProduct* PtrProduct = nullptr;
    for (uint32_t i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        BSTR PtrProductName = nullptr; // declare proper variable name for its use
        hr = PtrProduct->get_ProductName(&PtrProductName);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            std::cout << "Failed to query AV product name.";
            continue;
        }

        std::wstring displayName = std::wstring(PtrProductName, SysStringLen(PtrProductName));

        WSC_SECURITY_PRODUCT_STATE ProductState; // variable should be enclosed to its scope
        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product state.";
            continue;
        }

        std::wstring state;
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

        WSC_SECURITY_SIGNATURE_STATUS ProductStatus; // variable should be enclosed to its scope
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        std::string definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate"; // variable should be enclosed to its scope

        BSTR ProductStateTimestamp = nullptr; // declare proper variable name for its use
        hr = PtrProduct->get_ProductStateTimestamp(&ProductStateTimestamp);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        std::wstring timestamp = std::wstring(ProductStateTimestamp, SysStringLen(ProductStateTimestamp));
        SysFreeString(ProductStateTimestamp);

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    return (thirdPartyAVSoftwareMap.size() > 0); // simplicity
}