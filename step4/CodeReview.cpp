/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

//Add Headers
#include <string>
#include <map>
#include <optional>
#include <windows.h>
#include <iostream>
#include <iwscapi.h>
#include <atlbase.h>

#define OnErrorPrintAndExit(result, text, returnval) \
    if(FAILED(result)) {std::cout << text <<std::endl; return (returnval);}
#define OnErrorPrintAndContinue(result, text) \
    if(FAILED(result)) {std::cout << text <<std::endl; continue;}


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
    CComPtr<IWSCProductList> PtrProductList;
    hr = PtrProductList.CoCreateInstance(__uuidof(WSCProductList),NULL, CLSCTX_INPROC_SERVER);
    //Error handling is more than the main code, lets clean that
    OnErrorPrintAndExit(hr, "Failed to create WSCProductList object.", false)

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    OnErrorPrintAndExit(hr, "Failed to query antivirus product list." , false )

    LONG ProductCount = 0;
    hr = PtrProductList->get_Count(&ProductCount);
    OnErrorPrintAndExit(hr, "Failed to get the productCount" , false )

    
    //Keep loops short, better to separate this to its own function
    for (uint32_t i = 0; i < ProductCount; i++)
    {
        
        CComPtr<IWscProduct> PtrProduct = nullptr;
        hr = PtrProductList->get_Item(i, &PtrProduct);
        OnErrorPrintAndContinue(hr, "Failed to query AV product.");
        
        CComBSTR displayName = nullptr;
        hr = PtrProduct->get_ProductName(&displayName);
        OnErrorPrintAndContinue(hr, "Failed to query AV product name.");

        WSC_SECURITY_PRODUCT_STATE ProductState;
        hr = PtrProduct->get_ProductState(&ProductState);
        OnErrorPrintAndContinue(hr, "Failed to query AV product state.");

        std::wstring state;
        switch (ProductState) {
        case WSC_SECURITY_PRODUCT_STATE_ON:  
            state = L"On";  
            break;
        case WSC_SECURITY_PRODUCT_STATE_OFF:  
            state = L"Off";  
            break;
        default:
            state = L"Expired";
        }

        WSC_SECURITY_SIGNATURE_STATUS ProductStatus;
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        OnErrorPrintAndContinue(hr, "Failed to query AV product definition state.");
        
        //Define here
        std::string definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";
        CComBSTR timestamp = nullptr;
        hr = PtrProduct->get_ProductStateTimestamp(&timestamp);
        OnErrorPrintAndContinue(hr, "Failed to query AV product definition state.");

        ThirdPartyAVSoftware thirdPartyAVSoftware{
            .Name = displayName,
            .DefinitionStatus = definitionState,
            .DefinitionUpdateTime = timestamp,
            .Description = state,
            .ProductState = state
        };
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}