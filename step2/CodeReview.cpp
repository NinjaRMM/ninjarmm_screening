/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

/* Start Code review block
   Author: Ruben Mardones 
   Comment: Missing header files to make it build
   Action: Add the header files needed
*/
#include "iwscapi.h"
#include "wscapi.h"

#include <string>
#include <map>
#include <iostream>
/* End Code review block */

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
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        /* Start Code review block
           Author: Ruben Mardones 
           Comment: Possible memory leak here
           Action: Free allocated memory
        */
        if(PtrProductList != nullptr)
        {
            PtrProductList->Release();
        }
        /* End Code review block */
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        /* Start Code review block
           Author: Ruben Mardones 
           Comment: Possible memory leak here
           Action: Free allocated memory
        */
        if(PtrProductList != nullptr)
        {
            PtrProductList->Release();
        }
        /* End Code review block */
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        /* Start Code review block
           Author: Ruben Mardones 
           Comment: Possible memory leak here
           Action: Free allocated memory
        */
        if(PtrProductList != nullptr)
        {
            PtrProductList->Release();
        }
        /* End Code review block */
        return false;
    }
    
   
    /* Start Code review block
       Author: Ruben Mardones 
       Comment: Comparison betweeen unsigned and signed gives a warning
       Action: Replace uint32_t with LONG, to compare same type
    */
    for (LONG i = 0; i < ProductCount; i++)
    {
        /* End Code review block */
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            /* Start Code review block
               Author: Ruben Mardones 
               Comment: Possible memory leak here
               Action: Free allocated memory
            */
            if(PtrProduct != nullptr)
            {
                PtrProduct->Release();
                PtrProduct = nullptr;
            }
            /* End Code review block */
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            std::cout << "Failed to query AV product name.";
            /* Start Code review block
               Author: Ruben Mardones 
               Comment: Possible memory leak here
               Action: Free allocated memory
            */
            if(PtrProduct != nullptr)
            {
                PtrProduct->Release();
                PtrProduct = nullptr;
            }
            if(PtrVal != nullptr)
            {
                SysFreeString(PtrVal);
                PtrVal = nullptr;
            }
            /* End Code review block */
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product state.";
            /* Start Code review block
               Author: Ruben Mardones 
               Comment: Possible memory leak here
               Action: Free allocated memory
            */
            if(PtrProduct != nullptr)
            {
                PtrProduct->Release();
                PtrProduct = nullptr;
            }
            if(PtrVal != nullptr)
            {
                SysFreeString(PtrVal);
                PtrVal = nullptr;
            }
            /* End Code review block */
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
            /* Start Code review block
               Author: Ruben Mardones 
               Comment: Possible memory leak here
               Action: Free allocated memory
            */
            if(PtrProduct != nullptr)
            {
                PtrProduct->Release();
                PtrProduct = nullptr;
            }
            if(PtrVal != nullptr)
            {
                SysFreeString(PtrVal);
                PtrVal = nullptr;
            }
            /* End Code review block */
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            /* Start Code review block
               Author: Ruben Mardones 
               Comment: Possible memory leak here
               Action: Free allocated memory
            */
            if(PtrProduct != nullptr)
            {
                PtrProductList->Release();
                PtrProduct = nullptr;
            }
            if(PtrVal != nullptr)
            {
                SysFreeString(PtrVal);
                PtrVal = nullptr;
            }
            /* End Code review block */
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);
        /* Start Code review block
           Author: Ruben Mardones 
           Comment: Reset pointer to nullptr for next iteration
        */
        PtrVal = nullptr;
        /* End Code review block */

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
        /* Start Code review block
           Author: Ruben Mardones 
           Comment: Reset pointer to nullptr for next iteration
        */
        PtrProduct = nullptr;
        /* End Code review block */
    }

    /* Start Code review block
       Author: Ruben Mardones 
       Comment: Possible memory leak here
       Action: Free allocated memory
    */
    if(PtrProductList != nullptr)
    {
        PtrProductList->Release();
    }
    /* End Code review block */

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    
    return true;
}

/* Start Code review block
   Author: Ruben Mardones 
   Comment: Code for releasing memory is duplicated in many parts
   Action: TODO: Create a function to deallocate memory to avoid code duplication
*/
/* End Code review block */

int main()
{
    return 0;
}