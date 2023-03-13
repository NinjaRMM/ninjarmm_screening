#include <iostream>
#include <map>   /* Was missing this include */
#include <iwscapi.h>  /* Was missing this include */
#include <wscapi.h>  /* Was missing this include */

using namespace std;  /*Was missing this */

/*
NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.
Make any code updates that you see fit (If any).
Comments are encouraged.
*/
template<typename T>
void ReleasePointer(T* &ptr)
{
    if (ptr)
    {
        ptr->Release();
        ptr = nullptr;
    }
}
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

    hr = CoInitialize(NULL); /* Missing this call */

    if (FAILED(hr))
    {
        std::cout << "Failed CoInitialize. ";
        return false;
    }

    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
	    CoUninitialize();  /* Missing this call */
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        ReleasePointer(PtrProductList); /* Created a template class to release pointet */
        CoUninitialize();  /* Missing this call */
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        ReleasePointer(PtrProductList); /* Created a template class to release pointet */ /* Created a template class to release pointet */
        CoUninitialize();  /* Missing this call */
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
			PtrProduct = nullptr;  /* Good practice to set pointer to a null pointer after release */
            std::cout << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        SysFreeString(PtrVal); /* Missing SysFreeString call */
		PtrVal = nullptr;
		
        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
		    PtrProduct->Release(); /* was Missing this call */
			PtrProduct = nullptr;  /* Good practice to set pointer to a null pointer after release */
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
		    PtrProduct->Release(); /* was Missing this call */
			PtrProduct = nullptr;  /* Good practice to set pointer to a null pointer after release */
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
		    PtrProduct->Release(); /* was Missing this call */
			PtrProduct = nullptr;  /* Good practice to set pointer to a null pointer after release */
            std::cout << "Failed to query AV product definition state.";
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal); /* Missing SysFreeString call */
		PtrVal = nullptr;

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
		PtrProduct = nullptr; /* Good practice to set pointer to a null pointer after release */
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        ReleasePointer(PtrProductList); /* Created a template class to release pointet */ /* Created a template class to release pointet */
        CoUninitialize();  /* Missing this call */
        return false;
    }

    ReleasePointer(PtrProductList); /* Created a template class to release pointet */
    CoUninitialize();  /* Missing this call */
    return true;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}