/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/
//I clang-formatted the file

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string  DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

//Change the return value to better handle different error handling, my suggestion would be a enumerator
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    //Create return value variable here.
    //Reduce the scope of every other variable to keep as short as possible
    HRESULT                       hr             = S_OK;
    IWscProduct*                  PtrProduct     = nullptr;
    IWSCProductList*              PtrProductList = nullptr;
    BSTR                          PtrVal         = nullptr;
    LONG                          ProductCount   = 0;
    WSC_SECURITY_PRODUCT_STATE    ProductState;  //Not necessary to initialize cause it works, but I consider it a better practice. Also the name might cause confusion.
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus; //Not necessary to initialize cause it works, but I consider it a better practice

    // Rename "state" to something less generic so it'd be easier to find and avoid confusion
    std::wstring displayName, versionNumber, state, timestamp; //Avoid multiple variables in a single declaration. Also reduce the scope.
    std::string  definitionState;

    // To get rid of multiple return points and avoid increasing code depth I like to use "do {}while(0)"
    // Since its possible on each error define a variable to your return variable and "break"
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false; //remove multiple points of return. Create specific error for this
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        return false; //remove multiple points of return. Create specific error for this
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false; //remove multiple points of return. Create specific error for this
    }

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
            state = L"Expired"; //if "state" variable is created with this as the default value, this else could be erased
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

        //don't see the need for a temporary struct here. Can be accessed by the displayName and write directly

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name                          = displayName;
        thirdPartyAVSoftware.DefinitionStatus              = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime          = timestamp;
        thirdPartyAVSoftware.Description                   = state;
        thirdPartyAVSoftware.ProductState                  = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false; //remove multiple points of return. Create specific error for this
    }
    return true; //return the return value
}