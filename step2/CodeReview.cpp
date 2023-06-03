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
    IWSCProductList* PtrProductList = nullptr;

    HRESULT hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        if (PtrProductList != nullptr)
        {
            PtrProductList->Release();
        }
        logError("Failed to create WSCProductList object.", hr);
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        PtrProductList->Release();
        logError("Failed to query antivirus product list.", hr);
        return false;
    }

    LONG ProductCount = 0;
    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        PtrProductList->Release();
        logError("Failed to query product count.", hr);
        return false;
    }

    for (auto i = 0; i < ProductCount; i++)
    {
        IWscProduct* PtrProduct = nullptr;
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            if (PtrProduct != nullptr)
            {
                PtrProduct->Release();
            }
            logError("Failed to query AV product.", hr);
            continue;
        }

        BSTR PtrVal = nullptr;
        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            SysFreeString(PtrVal);  //No need to check for Null because it is a NOOP
            logError("Failed to query AV product name.", hr);
            continue;
        }

        std::wstring displayName = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        WSC_SECURITY_PRODUCT_STATE ProductState;
        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            logError("Failed to query AV product state.", hr);
            continue;
        }

        std::wstring state;
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

        WSC_SECURITY_SIGNATURE_STATUS ProductStatus;
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            logError("Failed to query AV product definition state.", hr);
            continue;
        }

        std::string definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            SysFreeString(PtrVal);  //No need to check for Null because it is a NOOP
            logError("Failed to query AV product definition state.", hr);
            continue;
        }
        std::wstring timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
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

    PtrProductList->Release();

    return !thirdPartyAVSoftwareMap.empty();
}

void logError(const std::string& text, const HRESULT& errorCode)
{
    std::cerr << text << " Error Code: " << errorCode << std::endl;
}
