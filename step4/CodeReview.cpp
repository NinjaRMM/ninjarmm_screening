
//DNV-2022-09-30 Must include this headers  
#include <iostream>
#include <string>
#include <map>
#include <iwscapi.h> //DNV-2022-09-30  https://learn.microsoft.com/en-us/windows/win32/api/iwscapi/nn-iwscapi-iwscproduct
#include <wscapi.h>  //DNV-2022-09-30  https://learn.microsoft.com/en-us/windows/win32/api/wscapi/ne-wscapi-wsc_security_provider
#include <string>
 

struct ThirdPartyAVSoftware
{
   // std::wstring Name;  //DNV-2022-09-30  Do not repeat, Use as the key in the [Key , value ] pair
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


    //DNV-2022-09-30  Must Initialize the COM library 

    HRESULT hrCoInit = CoInitialize(NULL);
   
    if (FAILED(hrCoInit))
    {
        std::cout << "Failed to Initialize COM";
        return false;
     }
    //END DNV-2022-09-30 

    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
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

    for (uint32_t i = 0; i < (ULONG)ProductCount; i++) // DNV 09-30-2022 No warning C4018: '<': signed/unsigned mismatch 
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
      //  thirdPartyAVSoftware.Name = displayName;
        
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;

        thirdPartyAVSoftwareMap[displayName] = thirdPartyAVSoftware; //DNV 2022-09-30 

        PtrProduct->Release();
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}



 

int main()
{
    //DNV-2022-09-30   Show Info 


    std::cout << " Step 4 \n";

     
    std::map<std::wstring, ThirdPartyAVSoftware> myThirdPartyAVSoftwareMap;
    if (queryWindowsForAVSoftwareDataWSC(myThirdPartyAVSoftwareMap))
    {

        std::cout << "-- AV Software Installed -- " << "\n";

         
        for (const auto& p : myThirdPartyAVSoftwareMap) {

            std::wcout << L"Name: " << p.first  << L" - Description: " << p.second.Description
                << L" - DefinitionUpdateTime: " << p.second.DefinitionUpdateTime;
            std::cout << " - DefinitionStatus: " << p.second.DefinitionStatus;
           // std::wcout << L" - Version: " << p.second.Version  // DNV 2022-09-30 No version number
            std::wcout  << L" - ProductState: " << p.second.ProductState     << L" \n";
           
         }
         
    }
    else
    {
        std::cout << " Not AV Software found !" << "\n";

    }



    getchar();
}
 