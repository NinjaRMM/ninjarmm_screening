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
    // 1) Is this field "DefinitionStatus" an internal string only and hence there would be no necessity for being declared as wstring ?
    // Maybe this string should be changed from std::string to the std::wstring format, like the others, just to standardize. 
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

// 2) At the end of the function, we return boolean based on the size of the map.
// Would it be usefull, instead, to return this actual map size, istead of a boolean? 
// Also, it would probably eliminate the need for the "if" clause (C/C++ interprets values == 0 as false, and values != 0 as true)
// 3) Rather, maybe we could think of refactoring to return HRESULT, as this function is doing query by means of COM
// 4) Maybe we could medidate a little bit about the name thirdPartyAVSoftwareMap for our parameter. 
// Is this really returning "3rd party software" info? Also, maybe rethink the name of the struct declared above, for the same reason.
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    // 5) We are assuming everything is running fine, but whenever a bad result appears, we signal and change hr object.
    // Would we be too much forcefull if we inverted logic (refactor), assuming things can go bad first, and then only let code flow further whenever HRESULTS are positive?
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    // 6) Maybe rename the following variable into something like "ProductSignatureStatus" or "SignatureStatus" in order avoid misinterpreting it as the "ProductState" variable, right above?
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    // 7) Variable versionNumber is declared right below, but it seems to never be used. Should it be excluded from declaration, or are we missing some "getVersion()" logic along the code?
    std::wstring displayName, versionNumber, state, timestamp;
    // 8) Maybe rethink this variable name (similar what was sugested for "ProductStatus" object (maybe rename it to something like "signatureDefinitionStatus").
    // Also, shouldn't this variable be wstring, as well as its counterpart at "ThirdPartyAVSoftware" struct?
    std::string definitionState;


    // 9) In acordance to Microsoft's documentation that second argument may be NULL - perfect. Anyways, shouldn't we use nullptr instead of NULL?
    // 10) Are we sure we wouldn't we have any undefined/unspecified behavior in our application by using reinterpret_cast?
    // What if we may break the casting operation: 1st) try to cast, 2nd) test the casting result, 3rd) and then after that (if everything goes ok), use that casted LPVOID result as *ppv argument to function CoCreateInstance?
    // 11) Also, according to documentation, we will need to manage the resource allocated by invoking CoCreateInstance() - shouldn't we separete responsibilities and create a specific class to do this for us (using Scope Bounded Resource Management (SBRM - also known as RAII)? This would probably help us in dealing better with exceptions, avoiding concurrency issues, resource leak, dangling pointers, etc.
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        // 12) In case of failure, would it be usefull to print the HR error code in hexadecimal?
        // 13) Wouldn't be better to throw exceptions in case of problem?
        // 14) If we insist in printing error, souldn't we use std::cerr buffer, instead of std::cout?
        // ** THIS COMMENT WOULD BE MADE FOR ALL OF THE OTHER FOLLOWING "if (FAILED(hr))" clauses, but I'll assume to do it here is enough
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

    // 15) This comparision is being made between signed vs unsigned data types. Couldn't we face any problem here?
    for (uint32_t i = 0; i < ProductCount; i++)
    {
        // 16) Maybe the instantiation of PtrProduct should be made, also, using SBRM (RAII) technique
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            // 17) In case we opt by not using SBRM (RAII), should this be really the only place where we invoke "PtrProduct->Release();" ?
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

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        // 18) Here, shouldn't we be atributing a proper "description" into the thirdPartyAVSoftware.Description field, instead of its "state"?
        // The way we made it, seems redundant. Is it usefull?
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}