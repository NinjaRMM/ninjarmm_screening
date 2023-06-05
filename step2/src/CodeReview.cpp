/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

// Maybe include "iwscapi.h" & "Windows.h" ??

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

// This looks like a function to obtain details of some products (AV) used by the Windows Security Center.
// At the end, after querying the IWSCProductList, the list of products are saved in the Map Container
// "thirdPartyAVSoftwareMap", with the "ThirdPartyAVSoftware" struct as the value element, and
// "ThirdPartyAVSoftware.Name" as the key element.
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr; // Pointer to individual WSC provider
    IWSCProductList* PtrProductList = nullptr; // Pointer to list of providers
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState; // enum for the state of the security product in the WSC
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus; // enum for the security signature status of the product

    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

    // In here, the parameter riid asks for a "IWSCProductList" interface, but at parameter ppv, even though "PtrProductList" is declared
    // as IWSCProductList*, a reinterpret_cast to LPVOID* is being done. Maybe this is necessary due to some COM limitation, but we
    // should check if this casting is avoidable...
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS); // Getting info about AV.
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        return false; // If the Initialize fails, you have to quit. You also can run the Initialize once
                      // per IWSCProductList (pointer) instance, and only one type of provider should be
                      // used. You can also use WSC_SECURITY_PROVIDER_ANTISPYWARE and WSC_SECURITY_PROVIDER_FIREWALL
                      // as provider type...
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false; // Without the count of AV providers we're unable to continue...
    }

    // Traverse product list
    for (uint32_t i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue; // get_Item() returns a Win32 error code? Next item in the list, please...
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release(); // Method inherited from IUnknown
            std::cout << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

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
        SysFreeString(PtrVal);

        // Once all validations are completed, the information is stored on the Map container.
        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release(); // Method inherited from IUnknown
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false; // Map is empty
    }
    return true; // Map is not empty, some product(s) information was gathered.
}
