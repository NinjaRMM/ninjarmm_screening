/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

// this had no includes, I'm so used to only seing code submitted that will actually compile before it's submitted for review
// that I didn't even think to check the most obvious thing
#include "CodeReview.h" // a source file without a header? that won't do
// STD includes
#include <iostream>
#include <map>
#include <string>

// General Windows API Include
#include <Windows.h>
// Specific Windows API Includes
#include <comdef.h>
#include <winerror.h>
#include <wscapi.h>
#include <iwscapi.h>

// Moved struct to header

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    std::wstring displayName; // Changed to single line declarations for readability and preference
    std::wstring state;
    std::wstring timestamp;
    std::wstring definitionState; // changed to wstring to match DefinitionStatus

    hr = CoInitialize(nullptr); // Gotta make sure the COM library is initialized
    if (FAILED(hr))
    {
        _com_error err(hr);
        std::cerr << "Failed to initialize COM library. Error " << std::hex << hr << " " << err.ErrorMessage(); //changed to std::cerr and added return code
        return false;
    }

    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        _com_error err(hr);
        std::cerr << "Failed to create WSCProductList object. Error " << std::hex << hr << " " << err.ErrorMessage(); //changed to std::cerr and added return code
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS); 
    if (FAILED(hr))
    {
        _com_error err(hr);
        std::cerr << "Failed to query antivirus product list. Error " << std::hex << hr << " " << err.ErrorMessage(); //changed to std::cerr and added return code
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        _com_error err(hr);
        std::cerr << "Failed to query product count. Error " << std::hex << hr << " " << err.ErrorMessage(); //changed to std::cerr and added return code
        return false;
    }

    for (uint32_t i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            _com_error err(hr);
            std::cerr << "Failed to query AV product. Error " << std::hex << hr << " " << err.ErrorMessage(); //changed to std::cerr and added return code
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            _com_error err(hr);
            PtrProduct->Release();
            std::cerr << "Failed to query AV product name. Error " << std::hex << hr << " " << err.ErrorMessage(); //changed to std::cerr and added return code
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            _com_error err(hr);
            std::cerr << "Failed to query AV product state. Error " << std::hex << hr << " " << err.ErrorMessage(); //changed to std::cerr and added return code
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
            _com_error err(hr);
            std::cerr << "Failed to query AV product definition state. Error " << std::hex << hr << " " << err.ErrorMessage(); //changed to std::cerr and added return code
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? L"UpToDate" : L"OutOfDate"; // changed inputs to match the new type.

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            _com_error err(hr);
            std::cerr << "Failed to query AV product definition state. Error " << std::hex << hr << " " << err.ErrorMessage(); //changed to std::cerr and added return code
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    PtrProductList->Release(); // missing release

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}
