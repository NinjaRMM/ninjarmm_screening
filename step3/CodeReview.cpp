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

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    // Moved the variables definitions closer to the first time they are actually used.
    // Removed the "versionNumber" variable as it never gets used.

    HRESULT hr = S_OK;
    IWSCProductList* ptrProductList = nullptr; // Matched naming convention.
    // Moved init statement inside if.
    if (hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList)); FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object."; // Removed space at the end to keep consistency.
        return false;
    }

    // Wraped IWSCProductList* into a std::unique_ptr with a custom deleter to avoid memory leaks and reduce the number of release calls.
    auto productListDeleter = [](IWSCProductList* ptrProductList)
                              {
                                  if (ptrProductList == nullptr) { return; }

                                  ptrProductList->Release(); // Added missing Release.
                              };

    std::unique_ptr<IWSCProductList, decltype(productListDeleter)> productList(ptrProductList, productListDeleter);

    // Moved init statement inside if.
    if (hr = productList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS); FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list."; // Removed space at the end to keep consistency.
        return false;
    }

    LONG productCount = 0; // Matched naming convention.
    // Moved init statement inside if.
    if (hr = productList->get_Count(&productCount); FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }

    // Fixed type mismatch. Use pre-increment instead of post-increment for i.
    for (uint32_t i = 0; i < static_cast<uint32_t>(productCount); ++i)
    {
        IWscProduct* ptrProduct = nullptr; // Matched naming convention.
        // Moved init statement inside if.
        if (hr = productList->get_Item(i, &ptrProduct); FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        // Wraped IWscProduct* into a std::unique_ptr with a custom deleter to avoid memory leaks and reduce the number of release calls.
        auto productDeleter = [](IWscProduct* ptrProduct)
                                {
                                    if (ptrProduct == nullptr) { return; }

                                    ptrProduct->Release();
                                };

        std::unique_ptr<IWscProduct, decltype(productDeleter)> product(ptrProduct, productDeleter);

        BSTR ptrProductName = nullptr; // Better variable name.
        // Moved init statement inside if.
        if (hr = product->get_ProductName(&ptrProductName); FAILED(hr))
        {
            // Removed Release call as we have a unique_ptr with a custom deleter now.
            std::cout << "Failed to query AV product name.";
            continue;
        }

        // Used const and parameterized constructor.
        const std::wstring displayName(ptrProductName, SysStringLen(ptrProductName));
        SysFreeString(ptrProductName); // Added missing SysFreeString. No point in creating a smart pointer out of it as we already have the value in the above wstring, so just free it.

        // Matched naming convention and initialized it to a default value. 'WSC_SECURITY_PRODUCT_STATE_OFF = 1' seems better than 'WSC_SECURITY_PRODUCT_STATE_ON = 0'.
        WSC_SECURITY_PRODUCT_STATE productState = WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_OFF;
        if (hr = product->get_ProductState(&productState); FAILED(hr))
        {
            std::cout << "Failed to query AV product state.";
            continue;
        }

        std::wstring state;
        // Used switch instead of chained ifs.
        switch (productState)
        {
            case WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_ON: // Added the enum name 'WSC_SECURITY_PRODUCT_STATE' before the value.
            {
                state = L"On";
                break;
            }
            case WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_OFF: // Added the enum name 'WSC_SECURITY_PRODUCT_STATE' before the value.
            {
                state = L"Off";
                break;
            }
            case WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_SNOOZED : // Added snoozed handling.
            {
                state = L"Snoozed";
                break;
            }
            case WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_EXPIRED : // Added dedicated expired handling
            {
                state = L"Expired";
                break;
            }
            default: // Added unknown handling
            {
                state = L"Unknown";
                break;
            }
        }

        WSC_SECURITY_SIGNATURE_STATUS productStatus _WSC_SECURITY_SIGNATURE_STATUS::WSC_SECURITY_PRODUCT_OUT_OF_DATE ; // Matched naming convention and initialized it to a default value. 
        if (hr = product->get_SignatureStatus(&productStatus); FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        std::string definitionState;
        // Used switch instead of ternary operator.
        switch (productStatus)
        {
            case _WSC_SECURITY_SIGNATURE_STATUS::WSC_SECURITY_PRODUCT_STATE_ON: // Added the enum name '_WSC_SECURITY_SIGNATURE_STATUS' before the value.
            {
                definitionState = "UpToDate";
                break;
            }
            case _WSC_SECURITY_SIGNATURE_STATUS::WSC_SECURITY_PRODUCT_STATE_OFF: // Added the enum name '_WSC_SECURITY_SIGNATURE_STATUS' before the value.
            {
                definitionState = "OutOfDate";
                break;
            }
            default: // Added unknown handling
            {
                definitionState = "Unknown";
                break;
            }
        }
        
        BSTR ptrTimestamp = nullptr; // Better variable name.
        // Moved init statement inside if.
        if (hr = product->get_ProductStateTimestamp(&ptrTimestamp); FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        // Used const and parameterized constructor.
        const std::wstring timestamp(ptrTimestamp, SysStringLen(ptrTimestamp));
        SysFreeString(ptrTimestamp); // No point in creating a smart pointer out of it as we already have the value in the above wstring, so just free it.

        // Use bracket init as any new members will be default initialized/constructed.
        ThirdPartyAVSoftware thirdPartyAVSoftware{};
        thirdPartyAVSoftware.Name = std::move(displayName); // Use move.
        thirdPartyAVSoftware.DefinitionStatus = std::move(definitionState); // Use move.
        thirdPartyAVSoftware.DefinitionUpdateTime = std::move(timestamp); // Use move.
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = std::move(state); // Use move.

        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = std::move(thirdPartyAVSoftware); // Use move.

        // Removed Release call as we have a unique_ptr with a custom deleter now.
    }

    // Check empty instead of size == 0.
    if (thirdPartyAVSoftwareMap.empty())
    {
        return false;
    }

    return true;
}