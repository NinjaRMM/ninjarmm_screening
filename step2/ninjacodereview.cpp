// ninjacodereview.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


/*
NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.
Make any code updates that you see fit (If any).
Comments are encouraged.
*/

// added the following include headers
#include <iostream>
#include <string>
#include <map>
#include <WinBase.h>
#include <iwscapi.h>
#include <Wscapi.h>

using namespace std;

struct ThirdPartyAVSoftware
{
    // use the namespace std
    wstring Name;
    wstring Description;
    wstring DefinitionUpdateTime;
    string DefinitionStatus;
    wstring Version;
    wstring ProductState;
};

bool queryWindowsForAVSoftwareDataWSC(map<wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    wstring displayName, versionNumber, state, timestamp;
    string definitionState;

    // Initialize can only be called once per instance
    // so, call CoCreateInstance for each security product type
    hr = CoCreateInstance(__uuidof(WSCProductList), 
                          NULL, 
                          CLSCTX_INPROC_SERVER, 
                          __uuidof(IWSCProductList), 
                          reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        cout << "Failed to create WSCProductList object: 0x" << hr << endl;
        return false;
    }

    // Initialize the product list with the type of security product
    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        cout << "Failed to query antivirus product list: 0x" << hr << endl;
        goto exit;
    }

    // get the number of security products of that type
    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        cout << "Failed to query product count: 0x" << hr << endl;
        goto exit;
    }

    // Loop over each product, querying the specific attributes
    for (uint32_t i = 0; i < ProductCount; i++)
    {
        // get the next security product
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            cout << "Failed to query AV product: 0x" << hr << endl;
            continue;
        }

        // get the product name
        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            cout << "Failed to query AV product name: 0x" << hr << endl;
            continue;
        }

        displayName = wstring(PtrVal, SysStringLen(PtrVal));

        // caller is responsible for freeing the string
        SysFreeString(PtrVal);
        PtrVal = nullptr;

        // get the product state
        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            cout << "Failed to query AV product state: 0x" << hr << endl;
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

        // get the signature status
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            cout << "Failed to query AV product definition state: 0x" << hr << endl;
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            cout << "Failed to query AV product definition state: 0x" << hr << endl;
            continue;
        }
        timestamp = wstring(PtrVal, SysStringLen(PtrVal));

        // caller is responsible for freeing the string
        SysFreeString(PtrVal);
        PtrVal = nullptr;

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
        PtrProduct = nullptr;
    }

    exit:

    if (nullptr != PtrProductList) {
        PtrProductList->Release();
    }
    if (nullptr != PtrProduct) {
        PtrProduct->Release();
    }
    if (nullptr != PtrVal) {
        SysFreeString(PtrVal);
    }
    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}