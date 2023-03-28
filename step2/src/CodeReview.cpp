/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

//Assuming all the header files are included except atlbase.h
//This is required to use Smart pointers for interfaces and BSTRs
#include <atlbase.h>

//Another way is import the .tlb file and use pointers and _bstrs_t

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
    //Assume always the function would fail, so negative is tested first
    HRESULT hr = E_FAIL;

    //Smart pointer to release automatically when ptr goes out of scope due to an error or success
    CComPtr<IWSCProductList> PtrProductList;
    //Easier way to create - no casting here
    hr = PtrProductList.CoCreateInstance(__uuidof(WSCProductList));
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

    //Move variable closer to usage/postpone as much as possible principle
    LONG ProductCount = 0;
    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }

    //Since LONG is what returned use this to avoid type mismatch warning
    for (LONG i = 0; i < ProductCount; i++)
    {

        //Smart pointer to release automatically when goes out of scope due to an error or success
        CComPtr<IWscProduct> PtrProduct;
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        //Move variable closer to usage & use Smart BSTR to manage memory resource automatically
        CComBSTR  PtrValProductName;
        hr = PtrProduct->get_ProductName(&PtrValProductName);
        if (FAILED(hr))
        {
            //removed PtrProduct->Release(); not needed anymore.
            std::cout << "Failed to query AV product name.";
            continue;
        }

        //No SysFreeString for earlier version: memory leak
        //removed unnecessary displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        //Move variable closer to usage
        WSC_SECURITY_PRODUCT_STATE ProductState;
        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product state.";
            continue;
        }

        //Move variable closer to usage
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

        //Move variable closer to usage
        WSC_SECURITY_SIGNATURE_STATUS ProductStatus;
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        //Move variable closer to usage
        std::string definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        //Move variable closer to usage & use Smart BSTR to manage memory resource automatically
        CComBSTR  PtrValTimeStamp;
        hr = PtrProduct->get_ProductStateTimestamp(&PtrValTimeStamp);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        //SysFreeString skipped in case of an error in the above call: memory leak
        //Removed unnecessary timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));

        //Directly assigned PtrValProductName & PtrValTimeStamp
        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = PtrValProductName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = PtrValTimeStamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
        
        //Removed, Not needed anymore: PtrProduct->Release();
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }

    return true;
}
