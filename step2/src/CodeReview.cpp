/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

// 1. Include necessary dependencies.

// 2. Considering that we are using C++, is there any reason to use struct and not use class instead?
struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    // 3. Why not using wstring here (below) as well? Using wstring will help to avoid casts in the future.
    std::string DefinitionStatus;
    // 4. Version is not being used.
    std::wstring Version;
    std::wstring ProductState;
};

// 5. Encapsulate this method into a class and use suitable modifiers.
// 6. This method populates a ThirdPartyAVSoftwareMap object with all antivirus (including Microsoft AV). Decide wheter
//      a product is thirdy party or not, or change the strucutre's name.
// 7. Add a header for this method specifying inputs, outputs, exceptions and behavior.
// 8. Avoid creating methods that you need to scroll down the window to see the entire content. Break this method into
//      more methods.
// 9. There are repetead behaviors in this function. Try to use some design pattern as Strategy Pattern.
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    // 10. Try to follow just one namming pattern. Ex.: camelCase, UperCamelCase, snake_case.
    // 11. Include necessary dependencies.
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    // 12. versionNumber never used.
    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

    // 13. At this point we don't know whether the COM server is intialized. So, let's handle the case it is not initialized yet.
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

    // 14. If the hr's are all FAILED, the code below will hide these failures and produce an empty list. 
    //      Try to inform the user (who is calling)
    //      the current method) that the list is empty due to internal failures. Ex.: throw an exception.
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
            // 15. Add a comment explainning why you are releasing here and what happen after that.
            PtrProduct->Release();
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
            // 16. Modify this message to be consistent with the error (something refering to definition state).
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            // 17. Modify this message to be consistent with the error (something refering to timestamp).
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        // 18. Perform this operation in a destructor
        SysFreeString(PtrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        // 19. State is being used twice, and the Version is never setted.
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        // 20. Perform this operation in a destructor
        PtrProduct->Release();
    }

    // 21. Return the result of direct comparision.
    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}