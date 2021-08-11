/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

#include "ObjBase.h"
#include "iwscapi.h"
#include "wscapi.h"
#include <string>
#include <map>
#include <iostream>

/* Include comments on the structure iself. 
    What the different members may contain in terms of values and their meaning.*/

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    /* Why is this defined differently than the others? Is this correct? */
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

/* Include comments as to what this function is attempting to do, expected parameters and return values and their meanings*/
/* Is a map the correct way to go? Is there a possibility that there could be more than one AV product with the same name?
    For example, two Norton AV products installed, but one is not active?*/

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
    
    /* Once again, this is defined differently? Correct or not?*/
    std::string definitionState;

    /* This call requires the thread to be STA. CoInitialize is not called within here, is it managed someplace else, or should it be managed in here? 
        If adding CoInitialize in here is done, CoUninitialize should be called as well. */

    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        /* Should this be to a log, or is stdout redirrected? Should the error be logged as well to aaid in debugging? */
        std::cout << "Failed to create WSCProductList object. ";
        /* If CoInitialize, CoUnitialize needs to happen before returning. */
        return false;
    }

    /* Check PtrProductList for existence. */

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        /* Should this be to a log, or is stdout redirrected? Should the error be logged as well to aaid in debugging? */
        std::cout << "Failed to query antivirus product list. ";
        /* If CoInitialize, CoUnitialize needs to happen before returning. */
        /* PtrProductList needs to be cleaned up before returning. */
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        /* Should this be to a log, or is stdout redirrected? Should the error be logged as well to aaid in debugging? */
        std::cout << "Failed to query product count.";
        /* If CoInitialize, CoUnitialize needs to happen before returning. */
        /* PtrProductList needs to be cleaned up before returning. */
        return false;
    }

    /* Compiler warning on below line. Signed/Unsigned mismatch */
    for (uint32_t i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            /* Should this be to a log, or is stdout redirrected? Should the error be logged as well to aaid in debugging? */
            std::cout << "Failed to query AV product.";
            /* PtrProductshould be checked and cleaned up if needed. */
            continue;
        }

        /* Check PtrProduct for existence. */

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            /* Should this be to a log, or is stdout redirrected? Should the error be logged as well to aaid in debugging? */
            std::cout << "Failed to query AV product name.";
            /* PtrVal be checked and cleaned up if needed. */
            continue;
        }

        /* Check PtrVal for existence. */

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        /* PtrVal be checked and cleaned up. */

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            /* Should this be to a log, or is stdout redirrected? Should the error be logged as well to aaid in debugging? */
            std::cout << "Failed to query AV product state.";
            continue;
        }

        /* switch statement more appropriate? */

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
            /* Should this be to a log, or is stdout redirrected? Should the error be logged as well to aaid in debugging? */
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            /* Should this be to a log, or is stdout redirrected? Should the error be logged as well to aaid in debugging? */
            std::cout << "Failed to query AV product definition state.";
            /* PtrVal be checked and cleaned up if needed. */
            continue;
        }

        /* Check PtrVal for existence. */

        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware;

        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;

        /* Check for prior existence before adding. */
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    /* If CoInitialize, CoUnitialize needs to happen before returning. */

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }

    return true;
}

int main() {
    
    CoInitialize(nullptr);

    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap;

    queryWindowsForAVSoftwareDataWSC(thirdPartyAVSoftwareMap);

    CoUninitialize();
}