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
    std::wstring Version; // unused
    std::wstring ProductState;
};

bool GetProductList(IWSCProductList** productList);
bool InitializeProductList(IWSCProductList* productList);
bool GetProductListCount(IWSCProductList* productList, long& count);
bool GetProduct(IWSCProductList* productList, long i, IWscProduct** product);
bool GetProductInfo(IWscProduct* product, ThirdPartyAVSoftware& software);
bool GetProductName(IWscProduct* product, std::wstring& outName);
bool GetProductState(IWscProduct* product, std::wstring& outState);
bool GetDefinitionStatus(IWscProduct* product, std::string& outStatus);
bool GetDefinitionUpdateTime(IWscProduct* product, std::wstring& outTime);

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    auto PtrProductList = (IWSCProductList *)nullptr;

    if (!GetProductList(&PtrProductList))
    {
        return false;
    }

    if (!InitializeProductList(PtrProductList))
    {
        return false;
    }

    auto ProductCount = 0L;
    if (!GetProductListCount(PtrProductList, ProductCount))
    {
        return false;
    }

    for (auto i = 0L; i < ProductCount; ++i)
    {
        ThirdPartyAVSoftware thirdPartyAVSoftware;
        auto PtrProduct = (IWscProduct *)nullptr;

        if (GetProduct(PtrProductList, i, &PtrProduct) &&
            GetProductInfo(PtrProduct, thirdPartyAVSoftware))
        {
            thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
        }

        PtrProduct->Release();
        PtrProduct = nullptr;
    }

    return (thirdPartyAVSoftwareMap.size() > 0);
}

bool GetProductList(IWSCProductList** productList)
{
    auto hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(productList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    return true;
}

bool InitializeProductList(IWSCProductList* productList)
{
    assert(productList != nullptr);

    auto hr = productList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    return true;
}

bool GetProductListCount(IWSCProductList* productList, long& count)
{
    assert(productList != nullptr);

    auto hr = productList->get_Count(&count);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }

    return true;
}

bool GetProduct(IWSCProductList* productList, long i, IWscProduct** product)
{
    assert(productList != nullptr);

    *product = nullptr;
    auto hr = productList->get_Item(i, product);
    if (FAILED(hr))
    {
        std::cout << "Failed to query AV product.";
        return false;
    }

    assert(product != nullptr);
    return true;
}

bool GetProductInfo(IWscProduct* product, ThirdPartyAVSoftware& software)
{
    assert(product != nullptr);

    if (!GetProductName(product, software.Name))
    {
        return false;
    }

    if (!GetProductState(product, software.ProductState))
    {
        return false;
    }
    software.Description = software.ProductState;

    if (!GetDefinitionStatus(product, software.DefinitionStatus))
    {
        return false;
    }

    if (!GetDefinitionUpdateTime(product, software.DefinitionUpdateTime))
    {
        return false;
    }

    return true;
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
