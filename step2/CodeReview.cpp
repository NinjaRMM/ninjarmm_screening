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
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    // I understand the approach of using a set of temporary variables to store the values to be 
    // assigned to thirdPartyAVSoftware later. However it could be interesting to consider
    // declaring thirdPartyAVSoftware at the beginning for-loop, and setting its member variables directly.
    // This approach could eliminate the need for the following temporary variables.
    std::wstring displayName, versionNumber, state, timestamp;

    
    // The following pattern is used repeatedly over the code:
    //      hr = func(...);
    //      if (FAILED(hr))
    //      {
    //          std::cout << "message";
    //          //some action
    //      }
    // To improve readability and avoid mistakes, it could be placed within a macro. 
    // Of course it is always better to avoid using macros, but sometimes if the it can make the code easier to read it could be considered. 
    // Here are examples of macros that could be used:
    //      #define SUCCESS_OR_RETURN(expr, msg) if(FAILED(expr)) {  std::cout << msg; return false; }
    //      #define SUCCESS_OR_CONTINUE(expr, msg) if(FAILED(expr)) { PtrProduct->Release(); std::cout << msg; continue; }
    // If this approach was to be adopted then the variable `hr` would not be necessary anymore. 
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

    // To avoid comparing variables with different types, 
    // the variable `i` could be changed to LONG.
    for (LONG i = 0; i < ProductCount; i++)
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
            PtrProduct->Release();
            std::cout << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));
        // As for timestamp, SysFreeString(PtrVal) is called here to free the string in PtrVal.
        SysFreeString(PtrVal);
        

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            // PtrProduct->Release() should be called here to release the object before continuing.
            PtrProduct->Release();
            std::cout << "Failed to query AV product state.";
            continue;
        }

        // The if else blocks could be replaced by a switch case block to make it more easily extensible.
        // Another option here would be to store the state strings on a std::unordered_map<WSC_SECURITY_PRODUCT_STATE, std::wstring> and 
        // use a default string as L"Expired".
        // Furthermore, since the state is stored in ProductState, this switch statement could be moved downwards, just before the 
        // declaration of thirdPartyAVSoftware. This could be done to allow all the data fetching to be separated from the data assignment. 
        // This is not done here to avoid confusion in the code review.
        switch (ProductState)
        {
        case WSC_SECURITY_PRODUCT_STATE_ON:
            state = L"On";
            break;
        case WSC_SECURITY_PRODUCT_STATE_OFF:
            state = L"Off";
            break;
        default:
            state = L"Expired";
            break;
        }

        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            // PtrProduct->Release() should be called here to release the object before continuing.
            PtrProduct->Release();
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            // PtrProduct->Release() should be called here to release the object before continuing.
            PtrProduct->Release();
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        // Since the definitionState information is only used here, it does not need to be stored in a variable.
        thirdPartyAVSoftware.DefinitionStatus = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    // To use a cleaner notation, the if block can be removed and the function can return
    // the evaluation of the logic expression.
    return thirdPartyAVSoftwareMap.size() != 0;
}