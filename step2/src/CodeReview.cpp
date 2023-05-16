/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

/** includes are needed even when the compiler could
 * automatically include them because it makes it easier
 * to see what dependencies the code and avoid issues changing
 * the compiler or compiler settings
*/
#include <iostream>
#include <string>
#include <map>
#include <exception>

// I am not sure if these are the proper includes as I am not familiar with the libraries
#include <combaseapi.h>
#include <iwscapi.h>
#include <wscapi.h>
#include <winerror.h>

struct ThirdPartyAVSoftware
{
    /**
     * There is not need of wstring if we are controlling the information wstring is bigger than regular string as
     * it could handle unicode characters, but we are not using unicode characters in this case, probably just for the name.
    */
    std::wstring Name;
    std::wstring Description; // description and product state are the same
    std::wstring DefinitionUpdateTime;
    /**
     * definition status could be an enum with more posible status instead of an string
     * this will make it easier to add new status types and take less memory.
     */
    std::string DefinitionStatus;
    std::wstring Version;  // Version is not being used
    std::wstring ProductState; // description and product state are the same
};

/**
* This function helps to create the ThirdPartyAVSoftware struct from the IWscProduct
* this separate the logic of creating the struct
*/
ThirdPartyAVSoftware createThirdPartyAVFromIWscProduct(IWscProduct* PtrProduct)
{
    BSTR PtrVal = nullptr;
    HRESULT hr = S_OK;
    ThirdPartyAVSoftware thirdPartyAVSoftware;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    hr = PtrProduct->get_ProductName(&PtrVal);
    if (FAILED(hr))
        throw std::runtime_error("Failed to query AV product name.");

    thirdPartyAVSoftware.Name = std::wstring(PtrVal, SysStringLen(PtrVal));

    hr = PtrProduct->get_ProductState(&ProductState);
    if (FAILED(hr))
        throw std::runtime_error("Failed to query AV product state.");

    // I prefer to handle these states with a switch instead of if else
    // but it is a personal preference and could depend on the coding style of the company
    switch (ProductState)
    {
        case WSC_SECURITY_PRODUCT_STATE_ON:
            thirdPartyAVSoftware.Description = L"On";
            thirdPartyAVSoftware.ProductState = L"On";
            break;
        case WSC_SECURITY_PRODUCT_STATE_OFF:
            thirdPartyAVSoftware.Description = L"Off";
            thirdPartyAVSoftware.ProductState = L"Off";
            break;
        default:
            thirdPartyAVSoftware.Description = L"Expired";
            thirdPartyAVSoftware.ProductState = L"Expired";
    }

    hr = PtrProduct->get_SignatureStatus(&ProductStatus);
    if (FAILED(hr))
        throw std::runtime_error("Failed to query AV product signature status.");

    // I prefer to handle the status with an enum allowing to add more status types
    thirdPartyAVSoftware.DefinitionStatus = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

    hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
    if (FAILED(hr))
        throw std::runtime_error("Failed to query AV product state timestamp.");

    thirdPartyAVSoftware.DefinitionUpdateTime = std::wstring(PtrVal, SysStringLen(PtrVal));
    SysFreeString(PtrVal);
    
    return thirdPartyAVSoftware;
}

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;

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
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }

    for (uint32_t i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        // using exceptions to handle errors instead of checking return value of the function
        // helps to avoid forgetting to check the return value and having unexpected behavior
        // in the code this behavior could be implemented in this function but as the signature
        // is unknown I am not sure if it is possible to change it.
        try {
            // calling the function to create the struct we are able to release the product when we are done with it oviding errors
            ThirdPartyAVSoftware thirdPartyAVSoftware = createThirdPartyAVFromIWscProduct(&thirdPartyAVSoftware, PtrProduct);
            thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
        } catch (const std::runtime_error& e)
            std::cout << e.what() << std::endl;

        PtrProduct->Release();
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}
