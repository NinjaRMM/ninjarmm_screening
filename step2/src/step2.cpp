/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

EXN: Please note, I haven't used these Windows libraries, so I am unfamilar with the APIs
EXN:  using these webpages:
  https://learn.microsoft.com/en-us/windows/win32/api/iwscapi/
  https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cocreateinstance

*/

#include <map>
#include <string>

typedef int HRESULT;

struct ThirdPartyAVSoftware
{
    // EXN: create a constructor that requires all elements, or all required elements
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
  // EXN: std::wstring versionNumber was not used, moved others down to limit scope

    IWSCProductList* PtrProductList = nullptr;
    HREESULT hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";  // EXN: might want std::endl here and on others
        return false;  // EXN: could consider nexted if statements to avoid multiple returns (or could possible use exceptions but that has other pitfalls)
                        //    same comment for similar code below
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr)) // EXN: might want to just check against S_OK based on API?  not sure on conventions however
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

    for (uint32_t i = 0; i < ProductCount; i++)
    {
        IWscProduct* PtrProduct = nullptr;
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        BSTR PtrProductName = nullptr; // EXN: use a better name
        hr = PtrProduct->get_ProductName(&PtrProductName);
        if (FAILED(hr))
        {
            PtrProduct->Release(); // EXN: I did not see anywhere that the reference count was incremented so far in the code
                                    //      so not sure if it needs to be released
                                    //      if it does need to be released, other continue statements below do not do it
                                    //      use nested ifs of exceptions
                                    //      some later continue statements have the same problem
            std::cout << "Failed to query AV product name.";
            continue;
        }

        WSC_SECURITY_PRODUCT_STATE ProductState;
        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product state.";
            // EXN: this leaks PtrProductName, use do not use continue, use nested ifs or exceptions
            //    some later continue statements have the same problem
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
        else // EXN: Missed handling the SNOOZED state
        {
            state = L"Expired";
        }

        WSC_SECURITY_SIGNATURE_STATUS ProductStatus;
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        // EXN: PtrVal already had the ProductName, use another string value for this (plus better names)
        BSTR ProductStateTimestamp = nullptr;
        hr = PtrProduct->get_ProductStateTimestamp(&ProductStateTimestamp);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        ThirdPartyAVSoftware thirdPartyAVSoftware;

        thirdPartyAVSoftware.Name = std::wstring(PtrProductName, SysStringLen(PtrProductName));
        SysFreeString(PtrProductName);

        thirdPartyAVSoftware.DefinitionStatus = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        thirdPartyAVSoftware.DefinitionUpdateTime = std::wstring(ProductStateTimestamp, SysStringLen(ProductStateTimestamp));
        SysFreeString(ProductStateTimestamp);

        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;

        // EXN: thirdPartyAVSoftware.Version is not initilized

        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    return thirdPartyAVSoftwareMap.size();
}
