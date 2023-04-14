/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

// Eliminate errors due to missing includes, etc in this code fragment
#define AVOID_MISSING_INCLUDE_ERRORS
#ifdef AVOID_MISSING_INCLUDE_ERRORS
#include <string>
#include <map>
#include <iostream>   // for cout
#include <winerror.h> // For HRESULT, S_OK
#include <comutil.h>  // for BSTR, LONG
#include <iwscapi.h>  // for WSC_...
const ULONG WSC_SECURITY_PROVIDER_ANTIVIRUS = 4;
#endif

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
    //void SetName(std::wstring name) {Name = name;}; // TODO: Add Getters/Setters if it doesn't interfere with users of the struct
};

// Setup functions (Creation, initialization, validation checks, etc) ================
bool CreateClassObject(IWSCProductList* PtrProductList = nullptr)
{
    HRESULT hr = S_OK;
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }
    return true;
}

bool InitProductList(IWSCProductList* PtrProductList = nullptr)
{
    HRESULT hr = S_OK;
    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }
    return true;
}

bool ValidateProductListCount(LONG &ProductCount, IWSCProductList* PtrProductList = nullptr)
{
    HRESULT hr = S_OK;
    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }
    return true;
}

bool GetProduct(IWSCProductList* PtrProductList, uint32_t index, IWscProduct* PtrProduct = nullptr)
{
    if( ! PtrProductList) return false;
    HRESULT hr = S_OK;
    hr = PtrProductList->get_Item(index, &PtrProduct);
    if (FAILED(hr))
    {
        std::cout << "Failed to query AV product.";
        return false;
    }
    return true;
}

bool GetProductName(IWscProduct* PtrProduct, BSTR PtrVal = nullptr)
{
    if( ! PtrProduct) return false;
    HRESULT hr = S_OK;
    hr = PtrProduct->get_ProductName(&PtrVal);
    if (FAILED(hr))
    {
        PtrProduct->Release();
        std::cout << "Failed to query AV product name.";
        return false;
    }
    return true;
}

bool GetProductState(IWscProduct* PtrProduct, WSC_SECURITY_PRODUCT_STATE ProductState)
{
    if( ! PtrProduct) return false;
    HRESULT hr = S_OK;
    hr = PtrProduct->get_ProductState(&ProductState);
    if (FAILED(hr))
    {
        std::cout << "Failed to query AV product state.";
        return false;
    }
    return true;
}

std::wstring GetProductStateString(WSC_SECURITY_PRODUCT_STATE ProductState)
{
    switch(ProductState)
    {
        case WSC_SECURITY_PRODUCT_STATE_ON:
            return L"On";
        case WSC_SECURITY_PRODUCT_STATE_OFF:
            return L"Off";
        default:
            return L"Expired";
    }
}

std::string GetProductStatusString(WSC_SECURITY_SIGNATURE_STATUS ProductStatus)
{
    return (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";
}

bool GetProductStatus(IWscProduct* PtrProduct, WSC_SECURITY_SIGNATURE_STATUS* ProductStatusPtr)
{
    if( ! PtrProduct) return false;
    HRESULT hr = S_OK;
    hr = PtrProduct->get_SignatureStatus(ProductStatusPtr);
    if (FAILED(hr))
    {
        std::cout << "Failed to query AV product definition state.";
        return false;
    }
    return true;
}

bool GetProductStateTS(IWscProduct* PtrProduct, BSTR PtrVal = nullptr)
{
    if( ! PtrProduct) return false;
    HRESULT hr = S_OK;
    hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
    if (FAILED(hr))
    {
        std::cout << "Failed to query AV product definition state.";
        return false;
    }
    return true;
}

std::wstring GetProductStateTsString(BSTR PtrVal)
{
    return std::wstring(PtrVal, SysStringLen(PtrVal));
}


//====================================================================================

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
    std::string definitionStatus;

    if( ! CreateClassObject(PtrProductList) ) return false;
    if( ! InitProductList(PtrProductList) ) return false;
    if( ! ValidateProductListCount(ProductCount, PtrProductList) ) return false;

    for (uint32_t i = 0; i < ProductCount; i++)
    {
        if( ! GetProduct(PtrProductList, i, PtrProduct)) continue;
        if( ! GetProductName(PtrProduct, PtrVal)) continue;
        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        if( ! GetProductState(PtrProduct, ProductState)) continue;
        state = GetProductStateString(ProductState);

        if( ! GetProductStatus(PtrProduct, &ProductStatus)) continue;
        definitionStatus = GetProductStatusString(ProductStatus);

        if( ! GetProductStateTS(PtrProduct, PtrVal)) continue;
        timestamp = GetProductStateTsString(PtrVal);
        SysFreeString(PtrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware; // TODO: Check for leak, also possible incomplete construction
        thirdPartyAVSoftware.Name = displayName; // TODO: Add GET/SET to struct assuming this doesn't interfere with useage
        thirdPartyAVSoftware.DefinitionStatus = definitionStatus;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}
