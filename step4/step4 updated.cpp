
struct ThirdPartyAVSoftware
{
    std::wstring name;
    std::wstring definitionUpdateTime;
    std::string definitionStatus;
    std::wstring version;
    std::wstring productState;
};

typedef std::map<std::wstring, ThirdPartyAVSoftware> ThirdPartyAVSoftwareMap_t;

bool QueryWindowsForAVSoftwareDataWSC(ThirdPartyAVSoftwareMap_t &thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;
    IWscProduct *ptrProduct = nullptr;
    IWSCProductList *ptrProductList = nullptr;
    BSTR ptrVal = nullptr;
    LONG productCount = 0;
    WSC_SECURITY_PRODUCT_STATE productState;
    WSC_SECURITY_SIGNATURE_STATUS signatureStatus;

    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionStatus;

    hr = CoCreateInstance(__uuidof(WSCProductList), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID *>(&ptrProductList));
    if (FAILED(hr))
    {
		LOGERROR("Failed to create WSCProductList object. ");
        return false;
    }

    hr = ptrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        LOGERROR("Failed to query antivirus product list. ");
        return false;
    }

    hr = ptrProductList->get_Count(&productCount);
    if (FAILED(hr))
    {
        LOGERROR("Failed to query product count.");
        return false;
    }

    for (uint32_t i = 0; i < productCount; i++)
    {
        hr = ptrProductList->get_Item(i, &ptrProduct);
        if (FAILED(hr))
        {
            LOGERROR("Failed to query AV product.");
            continue;
        }

        hr = ptrProduct->get_ProductName(&ptrVal);
        if (FAILED(hr))
        {
            ptrProduct->Release();
            LOGERROR("Failed to query AV product name.");
            continue;
        }

        displayName = std::wstring(ptrVal, SysStringLen(ptrVal));

        hr = ptrProduct->get_ProductState(&productState);
        if (FAILED(hr))
        {
            LOGERROR("Failed to query AV product state.");
            continue;
        }

        if (WSC_SECURITY_PRODUCT_STATE_ON == productState)
        {
            state = L"On";
        }
        else if (WSC_SECURITY_PRODUCT_STATE_OFF == productState)

        {
            state = L"Off";
        }
        else
        {
            state = L"Expired";
        }

        hr = ptrProduct->get_SignatureStatus(&signatureStatus);
        if (FAILED(hr))
        {
            LOGERROR("Failed to query AV product definition state.");
            continue;
        }

        definitionStatus = (WSC_SECURITY_PRODUCT_UP_TO_DATE == signatureStatus) ? "UpToDate" : "OutOfDate";

        hr = ptrProduct->get_ProductStateTimestamp(&ptrVal);
        if (FAILED(hr))
        {
            LOGERROR("Failed to query AV product definition state.");
            continue;
        }
		
        timestamp = std::wstring(ptrVal, SysStringLen(ptrVal));
        SysFreeString(ptrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.name = displayName;
        thirdPartyAVSoftware.definitionStatus = definitionStatus;
        thirdPartyAVSoftware.definitionUpdateTime = timestamp;
        thirdPartyAVSoftware.productState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.name] = thirdPartyAVSoftware;

        ptrProduct->Release();
    }

    if (thirdPartyAVSoftwareMap.empty())
    {
        return false;
    }
	
    return true;
}