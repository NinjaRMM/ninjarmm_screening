/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

#include <iwscapi.h>
#include <wscapi.h>
#include <string>
#include <map>
#include <iostream>
#include <functional>


struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

// Ideally, this would be at a helper header
template<class T>
std::unique_ptr<T, std::function<void(T*)>> make_com_unique(T* ptr)
{
    return std::unique_ptr<T, std::function<void(T*)>>(ptr, [](T* ptr) { ptr->Release(); });
}

/*
 * this next line is too big, I would suggest the following modification
+     bool queryWindowsForAVSoftwareDataWSC(
+       std::map<std::wstring, ThirdPartyAVSoftware> &thirdPartyAVSoftwareMap)
-     bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
*/
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

/*
 * this next line was too big, I would suggest the following modification
+     hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER,
+                           __uuidof(IWSCProductList),
+                           reinterpret_cast<LPVOID *>(&PtrProductList));
-     hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
*/
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER,
                          __uuidof(IWSCProductList),
                          reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        std::cerr << "Failed to create WSCProductList object. ";
        return false;
    }
    auto ProductList = make_com_unique(PtrProductList);

    hr = ProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cerr << "Failed to query antivirus product list. ";
        return false;
    }

    hr = ProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cerr << "Failed to query product count.";
        return false;
    }

    for (LONG i = 0; i < ProductCount; i++)
    {
        hr = ProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cerr << "Failed to query AV product.";
            continue;
        }
        auto Product = make_com_unique(PtrProduct);

        hr = Product->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            std::cerr << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        hr = Product->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            std::cerr << "Failed to query AV product state.";
            continue;
        }

        switch (ProductState) {
            case WSC_SECURITY_PRODUCT_STATE_ON:
                state = L"On";
                break;
            case WSC_SECURITY_PRODUCT_STATE_OFF:
                state = L"Off";
                break;
            default:
                state = L"Expired";
        }

        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            std::cerr << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            std::cerr << "Failed to query AV product definition state.";
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware{
            displayName,
            state,
            timestamp,
            definitionState,
            state
        };

        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
    }

    // change the next 5 lines to return thirdPartyAVSoftwareMap.size() != 0;
    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}

int main() {
    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap;
    queryWindowsForAVSoftwareDataWSC(thirdPartyAVSoftwareMap);
    for (const auto& sw : thirdPartyAVSoftwareMap) {
        std::cout << sw.first.c_str() << ": " << /* sw.second  */ "placeholder" << std::endl;
    }
    return EXIT_SUCCESS;
}
