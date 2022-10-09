/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/


#include "CodeReview.hpp"
#include "ThirdPartyAVSoftware.hpp"
#include "ScopedCoInitialize.hpp"

#include <atlbase.h>
#include <windows.h>
#include <iwscapi.h>
#include <Wscapi.h>
#include <iostream>
#include <optional>


// 1 - Breaking the code into smaller parts helps readability
// 2 - I prefer the output in return, but in this case, to improve readability, I chose to use an output parameter.
static bool parseProductName(IWscProduct* ptrProduct, ThirdPartyAVSoftware& thirdPartyAVSoftware);
static bool parseProductState(IWscProduct* ptrProduct, ThirdPartyAVSoftware& thirdPartyAVSoftware);
static bool parseProductStatus(IWscProduct* ptrProduct, ThirdPartyAVSoftware& thirdPartyAVSoftware);
static bool parseProductStateTimestamp(IWscProduct* ptrProduct, ThirdPartyAVSoftware& thirdPartyAVSoftware);
static std::optional<ThirdPartyAVSoftware> getAntiVirusInfo(IWSCProductList *ptrProductList, ULONG index);

bool AntiVirusInfoSeeker::queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    // CoInitialize has to be called in every single thread that uses COM
    auto scopedCoInitialize = ScopedCoInitialize();
    // Use a smart pointer class for managing COM interface pointers.
    CComPtr<IWSCProductList> ptrProductList;

    LONG productCount = 0;

    // 1 - 'hr' is too short, reduces readability
    // 2 - auto variables must be initialized, are generally immune to type mismatches that 
    //   can lead to portability or efficiency problems, can ease the process of refactoring, 
    //   and typically require less typing than variables with explicitly specified types
    // 3 - Use nullptr insted of NULL
    auto hResult = CoCreateInstance(
        __uuidof(WSCProductList),
        nullptr,
        CLSCTX_INPROC_SERVER,
        __uuidof(IWSCProductList),
        reinterpret_cast<LPVOID*>(&ptrProductList));

    if (FAILED(hResult))
    {
        // If this function is to be used in the future by a lib, I suggest not using stdout, 
        // perhaps a log when in debug mode, or returning the error to the function that calls it.
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hResult = ptrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hResult))
    {
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    hResult = ptrProductList->get_Count(&productCount);
    if (FAILED(hResult))
    {
        std::cout << "Failed to query product count.";
        return false;
    }

    // The types were different 
    for (LONG i = 0; i < productCount; i++) 
    {
        auto antiVirus = getAntiVirusInfo(ptrProductList, i);
        if (antiVirus) 
        {
            thirdPartyAVSoftwareMap[antiVirus.value().name] = antiVirus.value();
        }
    }

    // Use empty method here
    if (thirdPartyAVSoftwareMap.empty())
    {
        return false;
    }
    return true;
}

std::optional<ThirdPartyAVSoftware> getAntiVirusInfo(IWSCProductList *ptrProductList, ULONG index) 
{
    ThirdPartyAVSoftware thirdPartyAVSoftware;
    // Use a smart pointer class for managing COM interface pointers.
    CComPtr<IWscProduct> ptrProduct;

    auto hResult = ptrProductList->get_Item(index, &ptrProduct);
    if (FAILED(hResult))
    {
        std::cout << "Failed to query AV product." << std::endl;
        return {};
    }

    if (!parseProductName(ptrProduct, thirdPartyAVSoftware))
    {
        std::cout << "Failed to query AV product name." << std::endl;
        return {};
    }

    if (!parseProductState(ptrProduct, thirdPartyAVSoftware))
    {
        std::cout << "Failed to query AV product state." << std::endl;
        return {};
    }

    if (!parseProductStatus(ptrProduct, thirdPartyAVSoftware))
    {
        std::cout << "Failed to query AV product definition state." << std::endl;
        return {};
    }

    if (!parseProductStateTimestamp(ptrProduct, thirdPartyAVSoftware)) 
    {
        std::cout << "Failed to query AV product definition state." << std::endl;
        return {};
    }

    return {thirdPartyAVSoftware};
}

bool parseProductName(IWscProduct* ptrProduct, ThirdPartyAVSoftware& thirdPartyAVSoftware) 
{
    // Use a smart pointer class for managing COM interface pointers.
    CComBSTR ptrVal;

    auto hResult = ptrProduct->get_ProductName(&ptrVal);
    if (FAILED(hResult))
    {
        return false;
    }

    thirdPartyAVSoftware.name = std::wstring(ptrVal, SysStringLen(ptrVal));
    return true;
}

bool parseProductStatus(IWscProduct* ptrProduct, ThirdPartyAVSoftware& thirdPartyAVSoftware) 
{
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;
    auto hResult = ptrProduct->get_SignatureStatus(&ProductStatus);
    if (FAILED(hResult))
    {
        return false;
    }
    // Could be used enum here. Strong types helps avoid typos and reduces memory usage compared to strings
    thirdPartyAVSoftware.definitionStatus = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";
    return true;
}

bool parseProductState(IWscProduct* ptrProduct, ThirdPartyAVSoftware& thirdPartyAVSoftware)
{
    WSC_SECURITY_PRODUCT_STATE ProductState;
    auto hResult = ptrProduct->get_ProductState(&ProductState);
    if (FAILED(hResult))
    {
        return false;
    }
    if (ProductState == WSC_SECURITY_PRODUCT_STATE_ON)
    {
        thirdPartyAVSoftware.productState = L"On";
    }
    else if (ProductState == WSC_SECURITY_PRODUCT_STATE_OFF)
    {
        thirdPartyAVSoftware.productState = L"Off";
    }
    else 
    {
        thirdPartyAVSoftware.productState = L"Expired";
    }
    return true;
}

bool parseProductStateTimestamp(IWscProduct* ptrProduct, ThirdPartyAVSoftware& thirdPartyAVSoftware)
{
    // Use a smart pointer class for managing COM interface pointers.
    CComBSTR ptrVal;

    auto hResult = ptrProduct->get_ProductStateTimestamp(&ptrVal);
    if (FAILED(hResult))
    {
        return false;
    }
    thirdPartyAVSoftware.definitionUpdateTime = std::wstring(ptrVal, SysStringLen(ptrVal));
    return true;
}

