/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

/*
  Although this code review excercise has been exectued in a manner where I am making changes to the code and not only commenting on where I believe changes should be made, 
  I've also left comments on each change I made and why I felt it was necessary. Some of these comments could be interpreted as 'discussion starters' that I would consider 
  bringing up when reviewing other developers code, and would also be conversations I would like to have presented to me when others review my code.
*/

// FEEDBACK: includes were missing so the provided code would not compile. Added necessary includes.

// stl
#include <iostream>
#include <map>
#include <string>

// windows includes
#include <iwscapi.h>
#include <windows.h>
#include <wscapi.h>

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::wstring DefinitionStatus; // FEEDBACK: we want to use std::wstring rather thatn std::string in order to ensure we have space for wide characters that might be encountered in other languages on windows.
    std::wstring Version;
    std::wstring ProductState;
};

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    IWscProduct* PtrProduct = nullptr;
    std::wstring displayName; // FEEDBACK: break variable declarations up onto mulitple lines for cleanliness
    std::wstring versionNumber = L"0.0.0"; // FEEDBACK: this is never assigned to.
    std::wstring state; 
    std::wstring timestamp;
    std::wstring definitionState;

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

    // FEEDBACK: ProductCount is a long. Using uint_32 will work reliably on systems where long is defined to be 32bits, but this could vary. 
    // In the event this code was exectued on a system where long was 64bits, the iterator here could overflow resulting in a wrap-around causing an infinite loop.
    for (LONG i = 0; i < ProductCount; i++)
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

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? L"UpToDate" : L"OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        // FEEDBACK: it's cleaner and more efficient to populate the fields of our data structure here in line when we declare it.
        // FEEDBACK: versionNumber wasn't assigned to the field intended for it. 
        ThirdPartyAVSoftware thirdPartyAVSoftware
        {
            displayName,
            state,
            timestamp,
            definitionState,
            versionNumber,
            state
        };

        // FEEDBACK: depending on how this load function is used, we may be concerned with thirdPartyAVSoftwareMap already having contents. For instance,
        // if the caller called us once, then called again after installing another piece of software to update the map the passed map could already have contents.
        // In the event that we guarantee the map will be empty, it will be more efficient to use std::map::insert instead of the [] operator and an assignement to update the field. 
        // I'm going to leave it like it is because I don't have the use case knowledge to recommend the change.
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    return !thirdPartyAVSoftwareMap.empty(); // FEEDBACK it's much more concise to return the inverted empty() state of the map. It gets rid of an uncessesary branch too.
}

//temp just added so I could compile this as part of a VS project.

int main()
{
  std::map<std::wstring, ThirdPartyAVSoftware> myMap;
  queryWindowsForAVSoftwareDataWSC(myMap);
  return 0;
}