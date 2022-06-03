/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/


struct ThirdPartyAVSoftware
{
    std::wstring Name;
    // Removed the "Description" variable as it is being assigned the same value as ProductState.
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    // Removed the "Version" variable as it never gets used.
    std::wstring ProductState;
};

// Moved get antivirus product list to it's own function.
std::unique_ptr<IWSCProductList, std::function<void(IWSCProductList*)>> getAVProductList()
{
    HRESULT hr = S_OK;
    IWSCProductList* ptrProductList = nullptr; // Matched naming convention.
    // Moved init statement inside if.
    if (hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList)); FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. HRESULT: " << std::hex << hr << "." << std::endl; // Removed space at the end to keep consistency, added HRESULT code and endl.
        return {};
    }

    // Wraped IWSCProductList* into a std::unique_ptr with a custom deleter to avoid memory leaks and reduce the number of release calls.
    std::function<void(IWSCProductList*)> productListDeleter =
        [](IWSCProductList* ptrProductList)
        {
            if (ptrProductList == nullptr) { return; }

            ptrProductList->Release(); // Added missing Release.

            ptrProductList = nullptr;
        };

    std::unique_ptr<IWSCProductList, decltype(productListDeleter)> productList(ptrProductList, productListDeleter);

    // Moved init statement inside if.
    if (hr = productList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS); FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. HRESULT: " << std::hex << hr << "." << std::endl; // Removed space at the end to keep consistency, added HRESULT code and endl.
        return {};
    }

    return productList;
}

constexpr auto getAvProductStateString(WSC_SECURITY_PRODUCT_STATE productState)
{
    // Used switch instead of chained ifs.
    switch (productState)
    {
        // Added the enum name 'WSC_SECURITY_PRODUCT_STATE' before the value.
        case WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_ON: return L"On";
        // Added the enum name 'WSC_SECURITY_PRODUCT_STATE' before the value.
        case WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_OFF: return L"Off";
        // Added snoozed handling.
        case WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_SNOOZED: return L"Snoozed";
        // Added dedicated expired handling.
        case WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_EXPIRED: return L"Expired";
        // Added unknown handling.
        default: return L"Unknown";
    }
}

constexpr auto getAvProductSignatureStatusString(WSC_SECURITY_SIGNATURE_STATUS productStatus)
{
    // Used switch instead of ternary operator.
    switch (productStatus)
    {
        // Added the enum name 'WSC_SECURITY_SIGNATURE_STATUS' before the value.
        case WSC_SECURITY_SIGNATURE_STATUS::WSC_SECURITY_PRODUCT_STATE_ON: return "UpToDate";
        // Added the enum name 'WSC_SECURITY_SIGNATURE_STATUS' before the value.
        case WSC_SECURITY_SIGNATURE_STATUS::WSC_SECURITY_PRODUCT_STATE_OFF: return "OutOfDate";
        // Added unknown handling.
        default: return "Unknown";
    }
}

// Moved get antivirus software info to it's own function.
auto getAVSoftwareInfo(const IWscProduct &product, ThirdPartyAVSoftware &thirdPartyAVSoftware)
{
    HRESULT hr = S_OK;
    BSTR ptrProductName = nullptr; // Better variable name.
    // Moved init statement inside if and changed access operator.
    if (hr = product.get_ProductName(&ptrProductName); FAILED(hr))
    {
        // Removed Release call as we have a unique_ptr with a custom deleter now.
        std::cout << "Failed to query AV product name. HRESULT: " << std::hex << hr << "." << std::endl; // Added HRESULT code and endl.
        return false;
    }

    // Used parameterized constructor.
    std::wstring displayName(ptrProductName, SysStringLen(ptrProductName));
    SysFreeString(ptrProductName); // Added missing SysFreeString. No point in creating a smart pointer out of it as we already have the value in the above wstring, so just free it.

    // Matched naming convention and initialized it to a default value. 'WSC_SECURITY_PRODUCT_STATE_OFF = 1' seems better than 'WSC_SECURITY_PRODUCT_STATE_ON = 0'.
    WSC_SECURITY_PRODUCT_STATE productState = WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_OFF;
    // Moved init statement inside if and changed access operator.
    if (hr = product.get_ProductState(&productState); FAILED(hr))
    {
        std::cout << "Failed to query AV product state. HRESULT: " << std::hex << hr << "." << std::endl; // Added HRESULT code and endl.
        return false;
    }

    std::wstring state = getAvProductStateString(productState);

    WSC_SECURITY_SIGNATURE_STATUS productStatus = WSC_SECURITY_SIGNATURE_STATUS::WSC_SECURITY_PRODUCT_OUT_OF_DATE; // Matched naming convention and initialized it to a default value.
    // Moved init statement inside if and changed access operator.
    if (hr = product.get_SignatureStatus(&productStatus); FAILED(hr))
    {
        std::cout << "Failed to query AV product definition state. HRESULT: " << std::hex << hr << "." << std::endl; // Added HRESULT code and endl.
        return false;
    }

    std::string definitionState = getAvProductSignatureStatusString(productStatus);
    
    BSTR ptrTimestamp = nullptr; // Better variable name.
    // Moved init statement inside if and changed access operator.
    if (hr = product.get_ProductStateTimestamp(&ptrTimestamp); FAILED(hr))
    {
        std::cout << "Failed to query AV product definition state. HRESULT: " << std::hex << hr << "." << std::endl; // Added HRESULT code and endl.
        return false;
    }

    std::wstring timestamp(ptrTimestamp, SysStringLen(ptrTimestamp));
    SysFreeString(ptrTimestamp); // No point in creating a smart pointer out of it as we already have the value in the above wstring, so just free it.

    // Clear struct using bracket init as any new members will be default initialized/constructed.
    thirdPartyAVSoftware = ThirdPartyAVSoftware{};
    thirdPartyAVSoftware.Name = std::move(displayName); // Use move.
    thirdPartyAVSoftware.DefinitionStatus = std::move(definitionState); // Use move.
    thirdPartyAVSoftware.DefinitionUpdateTime = std::move(timestamp); // Use move.
    thirdPartyAVSoftware.Description = state;
    thirdPartyAVSoftware.ProductState = std::move(state); // Use move.

    return true;
}

// Moved get antivirus software map to it's own function. Pass productList as const ref.
std::map<std::wstring, ThirdPartyAVSoftware> getAVSoftwareMap(const IWSCProductList &productList)
{
    HRESULT hr = S_OK;
    LONG productCount = 0; // Matched naming convention.
    // Moved init statement inside if and changed access operator.
    if (hr = productList.get_Count(&productCount); FAILED(hr))
    {
        std::cout << "Failed to query product count. HRESULT: " << std::hex << hr << "." << std::endl; // Added HRESULT code and endl.
        return {};
    }

    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap;
    // Fixed type mismatch. Use pre-increment instead of post-increment for i.
    for (uint32_t i = 0; i < static_cast<uint32_t>(productCount); ++i)
    {
        IWscProduct* ptrProduct = nullptr; // Matched naming convention.
        // Moved init statement inside if and changed access operator.
        if (hr = productList.get_Item(i, &ptrProduct); FAILED(hr))
        {
            std::cout << "Failed to query AV product. HRESULT: " << std::hex << hr << "." << std::endl; // Added HRESULT code and endl.
            continue;
        }

        // Wraped IWscProduct* into a std::unique_ptr with a custom deleter to avoid memory leaks and reduce the number of release calls.
        std::function<void(IWscProduct*)> productDeleter =
            [](IWscProduct* ptrProduct)
            {
                if (ptrProduct == nullptr) { return; }

                ptrProduct->Release();

                ptrProduct = nullptr;
            };

        std::unique_ptr<IWscProduct, decltype(productDeleter)> product(ptrProduct, productDeleter);

        ThirdPartyAVSoftware thirdPartyAVSoftware{};
        if (!getAVSoftwareInfo(*product, thirdPartyAVSoftware))
        {
            continue;
        }

        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = std::move(thirdPartyAVSoftware); // Use move.

        // Removed Release call as we have a unique_ptr with a custom deleter now.
    }

    return thirdPartyAVSoftwareMap;
}

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    // Moved the variables definitions closer to the first time they are actually used.
    // Removed the "versionNumber" variable as it never gets used.

    const auto productList = getAVProductList();
    if (productList == nullptr)
    {
        return false;
    }

    thirdPartyAVSoftwareMap = getAVSoftwareMap(*productList);
    
    // Check empty instead of size == 0.
    if (thirdPartyAVSoftwareMap.empty())
    {
        std::cout << "No AV products detected." << std::endl; // Added log message.
        return false;
    }

    return true;
}