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

    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    
    /*
    * According to https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cocreateinstance
    * in this case PtrProductList could be returned as null, so before moving or doing anything else forward,
    * it would need to get validated. In this case, if the FAIL() macro properly handles it would be fine. But
    * if it does not, it will better to ask explicitly for the S_OK status on all instances where it applies
    */

    if (hr != S_OK)
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (hr != S_OK)
    {
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }
    /*
    * According to https://learn.microsoft.com/en-us/windows/win32/api/iwscapi/nf-iwscapi-iwscproductlist-get_count
    * method could fail returning a value different that S_OK. Asking explicitly that will avoid performing
    * operations on unwanted values.
    */
    hr = PtrProductList->get_Count(&ProductCount);
    if (hr != S_OK)
    {
        //
        // If not able to get the value, we would likely to free the memory as well
        // for the allocated product list.
        //
        PtrProductList->Release();
        std::cout << "Failed to query product count.";
        return false;
    }

    for (uint32_t i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (hr != S_OK)
        {
            std::cout << "Failed to query AV product.";
            continue;
        }
        //
        // Refer to https://learn.microsoft.com/en-us/windows/win32/api/iwscapi/nn-iwscapi-iwscproduct
        //
        hr = PtrProduct->get_ProductName(&PtrVal);
        if (hr != S_OK)
        {
            PtrProduct->Release();
            std::cout << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        hr = PtrProduct->get_ProductState(&ProductState);
        if (hr != S_OK)
        {
            //If it fails, allocated memory should be freed
            SysFreeString(PtrVal);
            PtrProduct->Release();
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
        if (hr != S_OK)
        {
            //If it fails, allocated memory should be freed
            SysFreeString(PtrVal);
            PtrProduct->Release();
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (hr != S_OK)
        {
            //If it fails, allocated memory should be freed
            SysFreeString(PtrVal);
            PtrProduct->Release();
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    //
    // Before returning, need to make sure the memory allocated is freed
    //
    PtrProductList->Release();
    
    /*
    * Just a quick style change. Although it is not really required.
    */
    return !thirdPartyAVSoftwareMap.empty();
}