/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

// I will asume that every single include which is needed is included. I asume that, because it seems like
// a chunk of code taken from a bigger part of the project.
// As this is not a PR, I cannot suggest changes to the code, so I will just write comments.
// NOTE: I do not know the style guide of the project, so I will try to figure it out from the code. This is
//       not the best way to do it, but it is the only way I can do it.


// as a general suggestion, Why not use a OOP approach?
// you can use a class to wrap the data and the functions that operate on it.

struct ThirdPartyAVSoftware // consider calling it AntiVirusSoftware, it's more descriptive.
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
    // consider using camelCase instead of CamelCase. A common convention is
    // to use camelCase for variable names and CamelCase for class names or types.
    // if the style guide says different, then use that.
    // why are you using std::wstring instead of std::string?
    // why is DefinitionStatus a std::string and not a std::wstring?
};

// why not use exceptions instead of returning false? If you do so, you can return ThirdPartyAVSoftware and after
// insert it in the map.
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr; // Consider using smart pointers instead of raw pointers.
    IWSCProductList* PtrProductList = nullptr; // Consider using smart pointers instead of raw pointers.
    BSTR PtrVal = nullptr; // Could you find a more descriptive name?
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;
    
    // consider using camelCase instead of CamelCase, for the above variables.
    std::wstring displayName, versionNumber, state, timestamp; // consider giving a default value to these variables.
                                                               // consider putting each variable in a separate line.
    std::string definitionState;
    // if the style guide doesn't say the contrary, cosider initializing the variables just before using them.

    // consider writting a function to handle CoCreateInstance and the error handling.
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList)); //this line is too long.
    if (FAILED(hr)) 
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    // consider writting a function to handle PtrProductList->Initialize and the error handling.
    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    // consider writting a function to handle PtrProductList->get_Count and the error handling.
    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }

    for (uint32_t i = 0; i < ProductCount; i++)
    {
        // consider writting a function to handle PtrProductList->get_Item and the error handling.
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        // consider writting a function to handle PtrProduct->get_ProductName and the error handling.
        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            std::cout << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        // consider writting a function to handle PtrProduct->get_ProductVersion and the error handling.
        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product state.";
            continue;
        }

        // consider writting a function to wrapp the state name giving logic.
        // consider using an unordered_map instead of a switch case, if memory is not a concern.
        // if memory is a concern, consider using a switch case.
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

        // consider writting a function to handle PtrProduct->get_ProductVersion and the error handling.
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        // consider writting a function to handle PtrProduct->get_ProductVersion and the error handling.
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
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
        // Are you sure that all the memory is being freed all the time? Can you ensure there are no memory leaks?
    }

    // This next lines just check if the map is empty, you may have not updated the map in the for loop.
    // Other outputs just tell you if there is an error. This violates the SRP principle.
    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}