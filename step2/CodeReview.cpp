/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

#include <combaseapi.h>
#include <iwscapi.h>
#include <wscapi.h>
#include <string>
#include <map>
#include <iostream>
#include <functional>


// Use wstring for everything
struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::wstring DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

// Ideally, this would be at a helper header
template<class T>
std::unique_ptr<T, std::function<void(T*)>> make_com_unique(T* ptr)
{
    return std::unique_ptr<T, std::function<void(T*)>>(ptr, [](T* ptr) { ptr->Release(); });
}

// This next line was too big
bool queryWindowsForAVSoftwareDataWSC(
    std::map<std::wstring, ThirdPartyAVSoftware> &thirdPartyAVSoftwareMap) {
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    std::wstring displayName, versionNumber, state, timestamp, definitionState;


    // This next line was too big
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER,
                          __uuidof(IWSCProductList),
                          reinterpret_cast<LPVOID*>(&PtrProductList));

    if (FAILED(hr))
    {
        // Change std::cout to std::cerr for failure messages
        std::cerr << "Failed to create WSCProductList object. ";
        return false;
    }

    // Use a unique_ptr with a custom deleter
    // to have Release called on its destruction.
    // PtrProductList was leaking.
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

    // There was a comparison between signed and unsigned numbers
    for (LONG i = 0; i < ProductCount; i++)
    {
        hr = ProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cerr << "Failed to query AV product.";
            continue;
        }
        // Use the same make_com_unique helper function to remove the need to
        // release the resource in several failure cases.
        // Some failure cases didn't release this pointer;
        auto Product = make_com_unique(PtrProduct);

        hr = Product->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            std::cerr << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));
        // The string wasn't released
        SysFreeString(PtrVal);

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

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? L"UpToDate" : L"OutOfDate";

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

    return thirdPartyAVSoftwareMap.size() != 0;
}

int main() {
    if (FAILED(CoInitialize(NULL)))
    {
        std::cerr << "Failed to initialize the COM Library.";
        return EXIT_FAILURE;
    }

    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap;
    if (queryWindowsForAVSoftwareDataWSC(thirdPartyAVSoftwareMap)) {
        std::cout << "Displaying installed antivirus" << std::endl << std::endl;
        for (const auto& sw : thirdPartyAVSoftwareMap) {
            std::wcout << sw.first << " ---- "
                << sw.second.Version << ", "
                << sw.second.ProductState << ", "
                << sw.second.DefinitionUpdateTime << ", "
                << sw.second.DefinitionStatus << ", "
                << sw.second.ProductState
                << std::endl;
        }
    }

    CoUninitialize();

    return EXIT_SUCCESS;
}
