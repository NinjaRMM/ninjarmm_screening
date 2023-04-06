/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

There are some main improvements to be considered:
    -It may be helpful to encapsulate this code within a class or a namespace in order to make it easier to reuse.
    -There is not logging or error handling. It could be done by returning an error code, throwing an exception or including a logger.
    -The naming conventions are not following any specific coding standards.
    -The code must include some documentation in order to make it easier to mantain.
    -The code makes use of raw pointers, which can lead to memory leaks and undefined behavior.
    -Lack of unit testing
*/

//The following headers are missing (Without them the code will not compile):
#include <iostream>
#include <map>
#include <winerror.h>
#include <wscapi.h> 


//Consider to include some docstrings for ThirdPartyAVSoftware. This can helps to make the code easier to mantain
struct ThirdPartyAVSoftware
{
    //Consider to use better names for the structure and its elements. I would recommend to read the book "Clean Code" from Robert Martin.
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

//Consider to include some docstrings and split this function in smaller methods.
//This can helps to make the code easier to mantain.
//The function does not handles exceptions. If something happens the program will crash.
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    //Consider to use camelCase names for local variables
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    std::wstring displayName, versionNumber, state, timestamp;
    //Consider to use a better name definitionState.
    std::string definitionState;

    //Considers to use a dynamic_cast instead of reinterpret_cast.
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        //Consider to manage the errors in a different way.
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        //Consider to manage the errors in a different way.
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        //Consider to manage the errors in a different way.
        std::cout << "Failed to query product count.";
        return false;
    }

    for (uint32_t i = 0; i < ProductCount; i++)
    {
        // There is a potential memory leak with PtrProductList which is not being released.
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            //Consider to manage the errors in a different way.
            std::cout << "Failed to query AV product.";
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            //Consider to manage the errors in a better way.
            std::cout << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            //Consider to manage the errors in a different way.
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
                //Consider to manage the errors in a different way.
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            //Consider to manage the errors in a different way.
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        //By definition State and Status are not the same. In this case, Status seems to be a better option
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        //Lets verify if thirdPartyAVSoftwareMap in null before trying to populate it.
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}