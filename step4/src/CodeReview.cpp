/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

//**************************************************************
// Adding header file to share structure and function signature 
//**************************************************************

#include "CodeReview.h"

//**************************************************************
// bool queryWindowsForAVSoftwareDataWSC()
// 
// Assumes COM has been initialized for the thread.
//**************************************************************

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    // objects
    IWscProduct*     PtrProduct     = nullptr;
    IWSCProductList* PtrProductList = nullptr;

    // system string
    BSTR PtrVal = nullptr;

    // count
    LONG ProductCount = 0;
    
    // security enums
    WSC_SECURITY_PRODUCT_STATE    ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    // misc strings
    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

    // create product list
    HRESULT hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    
    // if failed...
    if (FAILED(hr))
    {
        // output
        std::cerr << "Failed to create WSCProductList object. ";

        // return
        return false;
    }

    // init list
    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);

    // if failed...
    if (FAILED(hr))
    {
        // clean up
        PtrProductList->Release();

        // output
        std::cerr << "Failed to query antivirus product list. ";

        // return
        return false;
    }

    // get product count
    hr = PtrProductList->get_Count(&ProductCount);

    // if failed...
    if (FAILED(hr))
    {
        // clean up
        PtrProductList->Release();

        // output
        std::cerr << "Failed to query product count.";

        // return
        return false;
    }

    // for each product...
    for (uint32_t i = 0; i < uint32_t( ProductCount ); ++i)
    {
        // get product
        hr = PtrProductList->get_Item(i, &PtrProduct);

        // if failed...
        if (FAILED(hr))
        {
            // output
            std::cerr << "Failed to query AV product.";

            // next
            continue;
        }

        // get name
        hr = PtrProduct->get_ProductName(&PtrVal);

        // if failed...
        if (FAILED(hr))
        {
            // clean up
            PtrProduct->Release();

            // output
            std::cerr << "Failed to query AV product name.";

            // next
            continue;
        }

        // capture display name
        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));
        
        // clean up
        SysFreeString( PtrVal );

        // get product state
        hr = PtrProduct->get_ProductState(&ProductState);

        // if failed...
        if (FAILED(hr))
        {
            // clean up
            PtrProduct->Release();

            // output
            std::cerr << "Failed to query AV product state.";

            // next
            continue;
        }

        // if state on...
        if (ProductState == WSC_SECURITY_PRODUCT_STATE_ON)
        {
            // set it
            state = L"On";
        }

        // else off...
        else if (ProductState == WSC_SECURITY_PRODUCT_STATE_OFF)
        {
            // set it
            state = L"Off";
        }

        // else...
        else
        {
            // expired
            state = L"Expired";
        }

        // get signature status
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);

        // if failed...
        if (FAILED(hr))
        {
            // clean up
            PtrProduct->Release();

            // output
            std::cerr << "Failed to query AV product definition state.";

            // next
            continue;
        }

        // get uptodate string
        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        // get product timestamp
        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);

        // if failed...
        if (FAILED(hr))
        {
            // clean up
            PtrProduct->Release();

            // output
            std::cerr << "Failed to query AV product definition state.";

            // next
            continue;
        }
        
        // capture timestamp
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));

        // free system string
        SysFreeString(PtrVal);

        // init struct
        ThirdPartyAVSoftware thirdPartyAVSoftware = ThirdPartyAVSoftware();

        // set vals
        thirdPartyAVSoftware.Name                 = displayName;
        thirdPartyAVSoftware.DefinitionStatus     = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description          = state;
        thirdPartyAVSoftware.ProductState         = state;

        // add to map
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        // clean up
        PtrProduct->Release();
    }

    // clean up
    PtrProductList->Release();

    // if no entries...
    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        // output
        std::cerr << "No AV Products installed..";

        // return error
        return false;
    }

    // okay
    return true;
}