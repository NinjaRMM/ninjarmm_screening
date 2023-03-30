/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
//MODIFIED CODE is located at src/ModifiedCodeReview.cpp
Comments are encouraged.

*/

//added for browsing clarity
#include <Windows.h>
#include <string>
#include <map>
#include <iwscapi.h>
#include <wscapi.h>
#include <iostream>

//Overall Notes, good job, prelimenary testing shows it appears to work.
//Some memory leaks got through. Using smart ptrs can make this easier.
//Lots of duplicated code in error checking.
//No unit tests, and this isn't structured well for unit tests
struct ThirdPartyAVSoftware
{
    std::wstring Name;
    //REVIEW if description is not uniquely set maybe remove it from the structure?
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    //REVIEW is there a particular reasons this is set as string instead of wstring?
    std::string DefinitionStatus;
    //REVIEW if version is not set maybe remove it from the structure?
    std::wstring Version;
    std::wstring ProductState;
};

//Function is very long. We can help this by
// 1. pulling out the IWscProduct info gathering to a seperate function
// 2. Creating some kind of macro or function for error checking to reduce code duplication.
// 3. Consider using an exception to clean up code flow

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    //Legacy C style, we are allowed to declare variables close to where they are used now.
    HRESULT hr = S_OK;
    //strange to not use lower camel case or snake for local variables. but if allowed by ninjaone i'm fine.
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    //move this into the subfunction to get product_data and use a smart ptr
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    // Use ThirdPartyAVSoftware directly 
    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;
    //We should have a wrapper around this creation call to facillitate unit testing
    
    //nice use of reinterpret_cast to show clearly the hard conversion we must do due to the function signature.
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    //Good job remembering to check the error conditions of all these functions. We just have to make sure we don't leak memory in these cases.
    if (FAILED(hr))
    {   //is this cout intentional? consider cerr
        //consider rephrasing to "Could not crea..."
        //Unit tests may intentionally trigger this and people when searching for
        //"fail" in the build log will see these non erros
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    //the documentation says anything other than S_OK (0) is a falure. FAILED does not test for this
    // but all COM Error Codes fail for FAILED so should be ok. maybe worth investigating
    //https://learn.microsoft.com/en-us/windows/win32/api/iwscapi/nf-iwscapi-iwscproductlist-initialize
    if (FAILED(hr))
    {
        //leaks in these fail conditions
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }
    //signed unsigned mismatch
    for (uint32_t i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }
        //put the following code in a subfunction to reduce complexity of queryWindowsForAVSoftwareDataWSC
        //It would also allow us to get rid of some of those variables declared above and put them into the function
        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            std::cout << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));
        //if we are clever, we can wrap these calls in some kind of lambda since they are structurally identical
        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            //PtrProduct is leaked here and in subsecquent error clls
            //PtrVal is leaked here and in subsecquent error clls
            std::cout << "Failed to query AV product state.";
            continue;
        }
        //consider putting this in a seperate function. to reduce vertical complexity.
        //someone else may want to eventually convert these to strings
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
            //WSC_SECURITY_PRODUCT_STATE_SNOOZED is not explicitly handled
            //This incorporates the case of WSC_SECURITY_PRODUCT_STATE_SNOOZED
            //is this expected?
            state = L"Expired";
        }
        //if we are clever, we can wrap these calls in some kind of lambda since they are structurally identical
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            //PtrProduct is leaked here
            //PtrVal is leaked here.
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        //nice one liner
        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            //Whoops, this error message is for a failure to get_SignatureStatus. 
            //this should reflect a failure to get productstatetimestamp
            std::cout << "Failed to query AV product definition state.";
            //leaks
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        //why are Description and ProductState the same
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        //the map is passed in by reference. meaning we are potentially overriding data, intentional?
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
        
        PtrProduct->Release();
    }
    //clearer to use .empty()
    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    //PtrProductList is leaked 
    return true;
}
