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
    //REVIEW> It's better to declare the variables when using it, avoiding using it prematurely.
    HRESULT hr = S_OK;
    IWSCProductList* PtrProductList = nullptr;
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object.";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list.";
        return false;
    }

    LONG ProductCount = 0;
    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }

    //REVIEW> Avoid comparing signed with unsigned variables.
    for (LONG i = 0; i < ProductCount; i++)
    {
        IWscProduct* PtrProduct = nullptr;
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        BSTR PtrVal = nullptr;
        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            //REVIEW> As there are several exit points in this function, it would be better to wrap
            //        this PtrProduct in a new class to handle the release process on destructor,
            //        making the resource management less error prone.
            PtrProduct->Release();
            PtrProduct = nullptr;
            std::cout << "Failed to query AV product name.";
            continue;
        }
        std::wstring displayName(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);
        PtrVal = nullptr;

        WSC_SECURITY_PRODUCT_STATE ProductState = WSC_SECURITY_PRODUCT_STATE_OFF;
        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            //REVIEW> it was missing the PtrProduct release.
            PtrProduct->Release();
            PtrProduct = nullptr;
            std::cout << "Failed to query AV product state.";
            continue;
        }

        std::wstring state;
        //REVIEW> A switch statement is more appropriate for this kind of checking cases.
        switch (ProductState)
        {
        case WSC_SECURITY_PRODUCT_STATE_ON:
            state = L"On";
            break;
        case WSC_SECURITY_PRODUCT_STATE_OFF:
            state = L"Off";
            break;
        case WSC_SECURITY_PRODUCT_STATE_EXPIRED:
            //REVIEW> Checking for WSC_SECURITY_PRODUCT_STATE_EXPIRED as other types can be added
            //        to enumeration, causing other states to be considered as Expired.
            state = L"Expired";
            break;
        default:
            PtrProduct->Release();
            PtrProduct = nullptr;
            std::cout << "Unsupported AV product state.";
            continue;
        }

        WSC_SECURITY_SIGNATURE_STATUS ProductStatus = WSC_SECURITY_PRODUCT_OUT_OF_DATE;
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            PtrProduct = nullptr;
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        std::string definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            //REVIEW> it was missing the PtrProduct release.
            PtrProduct->Release();
            PtrProduct = nullptr;
            std::cout << "Failed to query AV product timestamp.";
            continue;
        }
        std::wstring timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);
        PtrVal = nullptr;

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.ProductState = state;
        //REVIEW> Description and Version fields are missing. Keeping the default empty string as
        //        they are not being set.
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
        PtrProduct = nullptr;
    }

    //REVIEW> Missing CoUninitialize.
    CoUninitialize();

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}
