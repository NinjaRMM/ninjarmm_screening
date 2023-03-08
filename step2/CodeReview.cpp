/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

/* review comments:
 * - Might consider throwing exception if thirdPartyAVSoftwareMap.size() == 0
 * - Changed variable names to be consistent in style
 * - Changed some strings to wstring
 * - Set default values for all variables
 * - thirdPartyAVSoftware.ProductState was set to state instead of productState
 */


struct ThirdPartyAVSoftware
{
    std::wstring Name {};
    std::wstring Description {};
    std::wstring DefinitionUpdateTime {};
    std::wstring DefinitionStatus {};
    std::wstring Version {};
    std::wstring ProductState {};
};

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr {S_OK};
    IWscProduct* ptrProduct {nullptr};
    IWSCProductList* ptrProductList {nullptr};
    BSTR ptrVal {nullptr};
    LONG productCount {0};
    WSC_SECURITY_PRODUCT_STATE productState {};
    WSC_SECURITY_SIGNATURE_STATUS productStatus {};

    std::wstring displayName {}, state {}, timestamp {}, definitionState {};

    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&ptrProductList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = ptrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    hr = ptrProductList->get_Count(&productCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }

    for (auto i = 0; i < productCount; i++)
    {
        hr = ptrProductList->get_Item(i, &ptrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        hr = ptrProduct->get_ProductName(&ptrVal);
        if (FAILED(hr))
        {
            ptrProduct->Release();
            std::cout << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(ptrVal, SysStringLen(ptrVal));

        hr = ptrProduct->get_ProductState(&productState);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product state.";
            continue;
        }

        if (productState == WSC_SECURITY_PRODUCT_STATE_ON)
        {
            state = L"On";
        }
        else if (productState == WSC_SECURITY_PRODUCT_STATE_OFF)
        {
            state = L"Off";
        }
        else
        {
            state = L"Expired";
        }

        hr = ptrProduct->get_SignatureStatus(&productStatus);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (productStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = ptrProduct->get_ProductStateTimestamp(&ptrVal);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        timestamp = std::wstring(ptrVal, SysStringLen(ptrVal));
        SysFreeString(ptrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = productState;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        ptrProduct->Release();
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}
