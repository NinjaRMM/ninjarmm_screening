/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

#include <string>
#include <iostream>
#include <map>

#define LONG int32_t
#define BSTR wchar_t*
#define CLSCTX_INPROC_SERVER 0
#define FAILED(hr) (hr != S_OK)
#define __uuidof(x) x
#define LPVOID void*
#define IID void*

#define HR_CHECK_FAIL(hr, fail_str) if(FAILED(hr)) { std::cout << fail_str; return false; }
#define HR_CHECK_CONTINUE(hr, fail_str) if(FAILED(hr)) { std::cout << fail_str; continue; }

class AbstractAVSoftware;

enum WSC_SECURITY_PRODUCT_STATE {
    WSC_SECURITY_PRODUCT_STATE_ON,
    WSC_SECURITY_PRODUCT_STATE_OFF,
    WSC_SECURITY_PRODUCT_UP_TO_DATE,
};

enum WSC_SECURITY_SIGNATURE_STATUS {
    WSC_SECURITY_PROVIDER_ANTIVIRUS
};

enum HRESULT {
    S_OK = 1,
};

static std::wstring getState(const WSC_SECURITY_PRODUCT_STATE ProductState) {
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

    return state;
};

static std::wstring getDefinitionState(const WSC_SECURITY_SIGNATURE_STATUS ProductStatus) {
    std::wstring definitionState;
    if (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE)
    {
        definitionState = "Up to date";
    }
    else
    {
        definitionState = "Out of date";
    }

    return definitionState;
};

struct IWSCProductList {
    virtual HRESULT Initialize(const WSC_SECURITY_SIGNATURE_STATUS ProductState) = 0;
    virtual HRESULT get_Count(LONG* ProductCount) = 0;
    virtual HRESULT get_Item(const LONG Index, IWscProduct** PtrProduct) = 0;
};

struct IWscProduct {
    virtual HRESULT get_ProductState(WSC_SECURITY_PRODUCT_STATE* ProductState) = 0;
    virtual HRESULT get_ProductName(BSTR* PtrVal) = 0;
    virtual HRESULT get_ProductDescription(BSTR* PtrVal) = 0;
    virtual HRESULT get_ProductStateTimestamp(BSTR* PtrVal) = 0;
    virtual HRESULT get_ProductUptoDateChecks(WSC_SECURITY_SIGNATURE_STATUS* ProductStatus) = 0;
    virtual HRESULT get_ProductVersion(BSTR* PtrVal) = 0;
    virtual HRESULT get_SignatureStatus(WSC_SECURITY_SIGNATURE_STATUS* ProductStatus) = 0;
    virtual void Release() = 0;
};

HRESULT CoCreateInstance(IID, void*, int, IID, void**);
HRESULT SysStringLen(BSTR);
HRESULT SysFreeString(BSTR);

// this is a monolithic function that does too much and spills implementation details that are hard to reason about at first glance - AAM
// are there any preconditions that need to be expressed for this function? - AAM
// why are we directly using the third party software instead of inverting the dependency and using an interface? - AAM
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, AbstractAVSoftware>& thirdPartyAVSoftwareMap)
{


    // Is there a reason why all these variables are declared at the top and are not scoped to the block where they are used? - AAM
    HRESULT hr = S_OK;
    // Are any of these owning pointers that can cause a memory leak? And otherwise should be smart pointer? - AAM
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    // Are two IWSCProductList uuids necessary? - AAM
    hr = CoCreateInstance(__uuidof(IWscProduct), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));

    HR_CHECK_FAIL(hr, "Failed to create WSCProductList object. ");

    // This initialization can't be part of the PtrProductList constructor? - AAM
    HR_CHECK_FAIL(PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS),
        "Failed to query antivirus product list. ");

    // How come we don't have an API that takes ProducCount by reference instead of a pointer? - AAM
    HR_CHECK_FAIL(PtrProductList->get_Count(&ProductCount), "Failed to query product count.");

    // This raw loop can be more expressive with for_each_n, then we can wrap up this logic in a lambda that's easier to reason about, reducing the monolith - AAM
    for (uint32_t i = 0; i < ProductCount; i++)
    {
        HR_CHECK_CONTINUE(PtrProductList->get_Item(i, &PtrProduct), "Failed to query AV product.");
        auto _ = gsl::finally([PtrProduct] { PtrProduct->Release(); });

        HR_CHECK_CONTINUE(PtrProduct->get_ProductName(&PtrVal),  "Failed to query AV product name.");

        std::wstring displayName = std::wstring(PtrVal, SysStringLen(PtrVal));
        auto __ = gsl::finally([PtrVal] { SysFreeString(PtrVal); });

        // It seems like this get_ProductState block can be wrapped up in its own function for easier readability - AAM
        HR_CHECK_CONTINUE(PtrProduct->get_ProductState(&ProductState), "Failed to query AV product state.");
        std::wstring definition(getDefinitionState(ProductStatus));

        std::wstring state(getState(ProductState));

        // variable declaration should be closer to where they are used - AAM
        HR_CHECK_CONTINUE(PtrProduct->get_SignatureStatus(&ProductStatus), "Failed to query AV product definition state.");
        HR_CHECK_CONTINUE(PtrProduct->get_ProductStateTimestamp(&PtrVal), "Failed to query AV product definition state.");

        auto timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));

        // It seems like most of this logic follows an adaptor pattern of mapping our own data structure to third party software - AAM
        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definition;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
    }

    return !thirdPartyAVSoftwareMap.empty();
}