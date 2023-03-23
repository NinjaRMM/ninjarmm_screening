/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

// Is this structure directly from the 3rd party SW? We should be inverting our dependencies here - AAM
struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

// this is a monolithic function that does too much and spills implementation details that are hard to reason about at first glance - AAM
// are there any preconditions that need to be expressed for this function? - AAM
// why are we directly using the third party software instead of inverting the dependency and using an interface? - AAM
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{

    // Is there a reason why all these variables are declared at the top and are not scoped to the block where they are used? - AAM
    HRESULT hr = S_OK;
    // Are any of these owning pointers that can cause a memory leak? And otherwise should be smart pointer? - AAM
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

    // There should probably be a comment explaining why this reinterpret_cast is safe - AAM
    // Is WSCProductList a typo? Should it be IWSCProductList? - AAM
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    // This is a lot of duplication code for error handling, can this be wrapped up in a macro of some error string? - AAM
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    // This initialization can't be part of the PtrProductList constructor? - AAM
    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    // How come we don't have an API that takes ProducCount by reference instead of a pointer? - AAM
    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }

    // This raw loop can be more expressive with for_each_n, then we can wrap up this logic in a lambda that's easier to reason about, reducing the monolith - AAM
    for (uint32_t i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            // Can RAII be leveraged instead of this manual release? Or even something like gsl::finally? - AAM
            PtrProduct->Release();
            std::cout << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        // It seems like this get_ProductState block can be wrapped up in its own function for easier readability - AAM
        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product state.";
            continue;
        }

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

        // variable declaration should be closer to where they are used - AAM
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));

        // Are we freeing dynamic memory here? If so, we should be using smart pointers to automatically handle cleanup- AAM
        SysFreeString(PtrVal);

        // It seems like most of this logic follows an adaptor pattern of mapping our own data structure to third party software - AAM
        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        // Ditto with respect to RAII - AAM
        PtrProduct->Release();
    }

    // Per the core guidelines we should consider being more expressive with post conditions
    // https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#i8-prefer-ensures-for-expressing-postconditions
    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}