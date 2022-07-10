/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

#include <string>
#include <map>
#include <wtypesbase.h>
#include <windows.h>
#include <iwscapi.h>
#include <sstream>
#include <iostream>
#include <stdint.h>

#ifndef WSC_SECURITY_PROVIDER_ANTIVIRUS
#define WSC_SECURITY_PROVIDER_ANTIVIRUS 0x04
#endif

/////////////////////////////////////////////////////////
//
// Please add the includes and defines necesary to compile
//

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

/////////////////////////////////////////////////////////
//
// Please use the camelCase convetion in local variables , all of them must start with lowercase
//

    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

/////////////////////////////////////////////////////////
//
// Please inizialize all the local variables
//


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

/////////////////////////////////////////////////////////
//
// Please cast ProductCount as uint32_t inside the for statement
//

    for (uint32_t i = 0; i < ProductCount; i++)
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
/////////////////////////////////////////////////////////
//
// Please Release PtrProduct
//
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
/////////////////////////////////////////////////////////
//
// Please Release PtrProduct
//
             continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
/////////////////////////////////////////////////////////
//
// Please Release PtrProduct
//
             continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);
/////////////////////////////////////////////////////////
//
// PtrVal are going to be release when PtrProduct will be released, 
// so please remove SysFreeString in order to avoid a heap corruption
//

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

/////////////////////////////////////////////////////////
//
//  Please release PtrProductList
//

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}

int main(int argc, char **argv)
	{
	}