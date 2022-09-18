/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

// Note: As you don't use '//' here for comments I will use it for my code review comments

// Note: I never have used this API

// To be able to compile this file locally
#if defined(kTEST_CodeReview)

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <Windows.h>
#include <iwscapi.h>
#include <wscapi.h>
#include <string>
#include <map>
#include <iostream>

#endif

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;           // Not fetched (set to ProductState)
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;       // Consider using WSC_SECURITY_SIGNATURE_STATUS
    std::wstring Version;               // Not fetched
    std::wstring ProductState;          // Consider using WSC_SECURITY_PRODUCT_STATE
};

// Consider using this struct for IWSCProductList and IWscProduct, then remove all the '->Release()' calls (you have forgotten some of them. This way is safer)
// For IWscProduct you have to reduce its scope to the for loop
template <typename Resource>
struct AutoRelease {
    ~AutoRelease() {
        if (ptr != nullptr) {
            ptr->Release();
            ptr = nullptr;
        }
    }

    Resource *operator->() const { return ptr; }

    Resource *ptr{nullptr};
};

// Consider reducing the length of this function
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    // Consider reducing the scope of the variables
    HRESULT hr = S_OK;
    //IWscProduct* PtrProduct = nullptr;    // If AutoRelease is used reducing the scope of this variable is mandatory
    //IWSCProductList* PtrProductList = nullptr;
    AutoRelease<IWSCProductList> PtrProductList;    // Using AutoRelease
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    // Consider using only one name convention for variables PascalCase, camelCase, snake_case, ...
    // VersionNumber is not used anywhere
    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

    // Consider put every parameter in a different line (It is clear for some Windows calls that have a lot of parameters)
    hr = CoCreateInstance(
        __uuidof(WSCProductList)
        , NULL
        , CLSCTX_INPROC_SERVER
        , __uuidof(IWSCProductList)
        , reinterpret_cast<LPVOID*>(&PtrProductList.ptr)    // In case you decided to use AutoRelease
    );
    if (FAILED(hr))
    {
        // If AutoRelease is not used PtrProductList must be released
        // PtrProductList->Release();

        // Consider using std::cerr instead
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        // If AutoRelease is not used PtrProductList must be released
        // PtrProductList->Release();

        // Consider using std::cerr instead
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        // If AutoRelease is not used PtrProductList must be released
        // PtrProductList->Release();

        // Consider using std::cerr instead
        std::cout << "Failed to query product count.";
        return false;
    }

    // It is better to use the same type
    //for (uint32_t i = 0; i < ProductCount; i++)
    for (LONG i = 0; i < ProductCount; i++)
    {
        AutoRelease<IWscProduct> PtrProduct;    // This variable must be here if you decide to use AutoRelease

        // Remove warning
        hr = PtrProductList->get_Item(ULONG(i), &PtrProduct.ptr);
        if (FAILED(hr))
        {
            // Consider using std::cerr instead
            std::cout << "Failed to query AV product.";
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            //PtrProduct->Release();    // Not needed due to AutoRelease

            // Consider using std::cerr instead
            std::cout << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        // PtrVal must be released
        SysFreeString(PtrVal);

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            // If AutoRelease is not used PtrProduct must be released
            // PtrProduct->Release();

            // Consider using std::cerr instead
            std::cout << "Failed to query AV product state.";
            continue;
        }

        // Consider using an enum for the state
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

        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            // If AutoRelease is not used PtrProduct must be released
            // PtrProduct->Release();

            // Consider using std::cerr instead
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            // If AutoRelease is not used PtrProduct must be released
            // PtrProduct->Release();

            // Consider using std::cerr instead
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        // Consider doing this instead to avoid copy data:
        ThirdPartyAVSoftware &thirdPartyAVSoftware = thirdPartyAVSoftwareMap[displayName];
        //ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        //thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        //PtrProduct->Release();        // Not needed due to AutoRelease
    }

    // Consider using !empty
    //if (thirdPartyAVSoftwareMap.size() == 0)
    //{
    //    return false;
    //}
    //return true;
    return !thirdPartyAVSoftwareMap.empty();
}

// To be able to compile this code locally
#if defined(kTEST_CodeReview)

int
main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap;

    HRESULT hr = CoInitialize(nullptr);
    if (SUCCEEDED(hr)) {
        if (queryWindowsForAVSoftwareDataWSC(thirdPartyAVSoftwareMap)) {
            for (const auto &[key, value] : thirdPartyAVSoftwareMap) {
                std::wcout << key << "\n";
                std::wcout << "  - Description: "          << value.Description          << "\n";
                std::wcout << "  - DefinitionUpdateTime: " << value.DefinitionUpdateTime << "\n";
                std::cout  << "  - DefinitionStatus: "     << value.DefinitionStatus     << "\n";
                std::wcout << "  - Version: "              << value.Version              << "\n";
                std::wcout << "  - ProductState: "         << value.ProductState         << "\n";
            }
        }
        CoUninitialize();
    }


    return 0;
}

#endif
