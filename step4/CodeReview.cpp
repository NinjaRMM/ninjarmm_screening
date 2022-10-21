/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/
#include <iostream>
#include <cstdio>
#include <map>


struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};



/*
 * This function has a lot of responsabilities, maybe we should break it in more functions in order to make each function 
 * responsible for something smaller, it eases the manteinability and readability of the code.
 *
 * 1 - The return of the function is not clear on its purpose, maybe it's better to return an integer with 
 *     a status code, given the amount of possible return errors, another approach would be the use of exceptions.
 *
 * 2 - The v ariable hr is being used on all the calls, maybe the return of it is discardable? 
 *
 * 3 - The code is pretty ugly and hard to understand, we could have a cleaner code in order to understand better what is happening.
 *  Ex:
 *  Instead of a big chunk of code in one line like this:
 * 
 *   hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
 *
 *   we can break in multiple lines like this:
 *
    hr = CoCreateInstance(__uuidof(WSCProductList), 
	                      NULL, 
						  CLSCTX_INPROC_SERVER, 
						  __uuidof(IWSCProductList), 
						  reinterpret_cast<LPVOID*>(&PtrProductList));

	Its easy to read and to know each argument of the function.

 * 4 - The code lacks patterns:
 * 		IWscProcuct and IWSCProductList -> should be IWSCProduct or IWscProductList.
 * 		A lot of uknown types, enums and classes (probably will come from another file).
 *
 *
 */


// I've tried to recreate some of the headers in order to understand the code better, however i was not able to do it perfectly.
// Enums/Types definition
typedef int HRESULT;
typedef long LONG;
typedef std::string BSTR;
typedef void LPVOID;
typedef int WSC_SECURITY_PRODUCT_STATE;
typedef int WSC_SECURITY_SIGNATURE_STATUS;


// This should probably be an enum
int S_OK = 0;
int CLSCTX_INPROC_SERVER = 1;
int WSC_SECURITY_PROVIDER_ANTIVIRUS = 0;
int WSC_SECURITY_PRODUCT_STATE_ON = 1;
int WSC_SECURITY_PRODUCT_STATE_OFF = 0;
int WSC_SECURITY_PRODUCT_UP_TO_DATE = 0;

// Functions
int __uidof(void * t);
bool FAILED(HRESULT);
int SysStringLen(BSTR); // this is missing some parameter that im not sure what.
void SysFreeString(BSTR);
void CoCreateInstance();

// Class definition
//
class IWscProduct {
	public:
	HRESULT get_ProductName(BSTR*a);
	void Release();
	HRESULT get_ProductState(WSC_SECURITY_PRODUCT_STATE *);
	HRESULT get_ProductStateTimestamp(BSTR *);
	HRESULT get_SignatureStatus(WSC_SECURITY_SIGNATURE_STATUS *);
};
class IWSCProductList {
	public:

	HRESULT Initialize(int );
	HRESULT get_Count(LONG *);
	HRESULT get_Item(int, LONG *);
	HRESULT get_Item(uint32_t & a, IWscProduct ** b);
};

// 
// End of definitions
//
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;

	// IWsc and IWSC -> may we should maintain standard names.
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
	// Im not sure why it's using LONG, the standard provides long.
    LONG ProductCount = 0;

    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

    hr = CoCreateInstance(__uuidof(IWSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
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
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}
