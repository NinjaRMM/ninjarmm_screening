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
    // Consider using the same naming convention accross all variables.
    // These right below are all in PascalCase where as the last set are in camelCase. 
    // This is referring to everything between line 29 and 38, also within the ThirdPartyAVSoftare struct
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    // versionNumber is never used within this function so should either be used or removed.
    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

    // Consider putting each variable for this very long function call on seperate lines for an easier read.
    hr = CoCreateInstance(__uuidof(WSCProductList), 
                          NULL, 
                          CLSCTX_INPROC_SERVER, 
                          __uuidof(IWSCProductList), 
                          reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        // Consider using cerr instead of cout as this is an error statement.
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        // Consider using cerr instead of cout as this is an error statement.
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        // Consider using cerr instead of cout as this is an error statement.
        std::cout << "Failed to query product count.";
        return false;
    }

    // Consider using LONG instead of uint32_t to keep in line with hr's type.
    // However it depends on what the parameters are for the PtrProductList->get_Item() function.
    // If that function specifically calls for a uint32_t to be passed in the leave as is.
    for (uint32_t i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            // Consider using cerr instead of cout as this is an error statement.
            std::cout << "Failed to query AV product.";
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            // Consider using cerr instead of cout as this is an error statement.
            std::cout << "Failed to query AV product name.";
            continue;
        }

        //Need to use SysFreeString after the SysStringLen call.
        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            // Consider using cerr instead of cout as this is an error statement.
            std::cout << "Failed to query AV product state.";
            continue;
        }

        // Consider using a switch statement for this comparison and switching the type of ProductState to be an enum
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
            // Consider using cerr instead of cout as this is an error statement.
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            // Consider using cerr instead of cout as this is an error statement.
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        // Consider working with the reference included in the input map as to not copy data unecessarily.
        // ThirdPartyAVSoftware thirdPartyAVSoftware;
        ThirdPartyAVSoftware &thirdPartyAVSoftware = thirdPartyAVSoftwareMap[displayName];
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        //thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    // Consider simplifying this to use empty()
    //if (thirdPartyAVSoftwareMap.size() == 0)
    //{
        //return false;
    //}
    //return true;
    return !thirdPartyAVSoftwareMap.empty();
}