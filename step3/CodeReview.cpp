/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

#include <cassert>
#include <iostream>
#include <map>
#include <string>

#include <Windows.h>
#include <iwscapi.h>
#include <wscapi.h>

struct ThirdPartyAVSoftware // move this to a header?
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

bool GetProductName(IWscProduct* product, std::wstring& outName);
bool GetProductState(IWscProduct* product, std::wstring& outState);
bool GetDefinitionStatus(IWscProduct* product, std::string& outStatus);
bool GetDefinitionUpdateTime(IWscProduct* product, std::wstring& outTime);

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    auto hr = S_OK;
    auto PtrProductList = (IWSCProductList *)nullptr;

    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
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

    LONG ProductCount = 0;
    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }

    for (auto i = 0L; i < ProductCount; i++)
    {
        ThirdPartyAVSoftware thirdPartyAVSoftware;

        auto PtrProduct = (IWscProduct *)nullptr;
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        if (!GetProductName(PtrProduct, thirdPartyAVSoftware.Name))
        {
            PtrProduct->Release();
            PtrProduct = nullptr;
            continue;
        }

        if (!GetProductState(PtrProduct, thirdPartyAVSoftware.ProductState))
        {
            PtrProduct->Release();
            PtrProduct = nullptr;
            continue;
        }
        thirdPartyAVSoftware.Description = thirdPartyAVSoftware.ProductState;

        if (!GetDefinitionStatus(PtrProduct, thirdPartyAVSoftware.DefinitionStatus))
        {
            PtrProduct->Release();
            PtrProduct = nullptr;
            continue;
        }

        if (!GetDefinitionUpdateTime(PtrProduct, thirdPartyAVSoftware.DefinitionUpdateTime))
        {
            PtrProduct->Release();
            PtrProduct = nullptr;
            continue;
        }

        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
        PtrProduct = nullptr;
    }

    return (thirdPartyAVSoftwareMap.size() > 0);
}

bool GetProductName(IWscProduct* product, std::wstring& outName)
{
    assert(product != nullptr);

    auto PtrVal = BSTR(nullptr);
    auto hr = product->get_ProductName(&PtrVal);
    if (FAILED(hr))
    {
        std::cout << "Failed to query AV product name.";
        return false;
    }

    assert(PtrVal != nullptr);
    outName = std::wstring(PtrVal, SysStringLen(PtrVal));
    SysFreeString(PtrVal);
    PtrVal = nullptr;

    return true;
}

bool GetProductState(IWscProduct* product, std::wstring& outState)
{
    assert(product != nullptr);

    auto ProductState = WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_EXPIRED;
    auto hr = product->get_ProductState(&ProductState);
    if (FAILED(hr))
    {
        std::cout << "Failed to query AV product state.";
        return false;
    }

    switch (ProductState)
    {
    case WSC_SECURITY_PRODUCT_STATE_ON:
        outState = L"On";
        break;
    case WSC_SECURITY_PRODUCT_STATE_OFF:
        outState = L"Off";
        break;
    default:
        // WSC_SECURITY_PRODUCT_STATE_SNOOZED - is this intentional?
        // WSC_SECURITY_PRODUCT_STATE_EXPIRED
        outState = L"Expired";
        break;
    }

    return true;
}

bool GetDefinitionStatus(IWscProduct* product, std::string& outStatus)
{
    assert(product != nullptr);

    auto ProductStatus = WSC_SECURITY_SIGNATURE_STATUS::WSC_SECURITY_PRODUCT_OUT_OF_DATE;
    auto hr = product->get_SignatureStatus(&ProductStatus);
    if (FAILED(hr))
    {
        std::cout << "Failed to query AV product definition state.";
        return false;
    }

    outStatus = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ?
        std::string("UpToDate") : std::string("OutOfDate");
    return true;
}

bool GetDefinitionUpdateTime(IWscProduct* product, std::wstring& outTime)
{
    assert(product != nullptr);

    auto PtrVal = BSTR(nullptr);
    auto hr = product->get_ProductStateTimestamp(&PtrVal);
    if (FAILED(hr))
    {
        std::cout << "Failed to query AV product definition state.";
        return false;
    }

    assert(PtrVal != nullptr);
    outTime = std::wstring(PtrVal, SysStringLen(PtrVal));
    SysFreeString(PtrVal);
    PtrVal = nullptr;

    return true;
}
