/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/
#include <string>
#include <map>

using namespace std;

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

    //Code comment by Alex Joseph:
    // 1. Need to log different return codes to differentiate between different types of failures.
    // 2. Also need to log to system wide logger instead of stdout. Assuming a singleton logger 'Logger' exists in the application.
    // 3. Consider using code that cleans up memory allocation automatically when the variable goes out of scope.
    //     Option a) Use something similar to scope_gard for RAII https://ricab.github.io/scope_guard/.
    //	   Option b) Associate custom "free" function with std::unique_ptr. Eg: using scoped_ptr_PtrProduct = std::unique_ptr<PtrProduct, decltype(&PtrProduct->Release)>;
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        Logger::getInstance->getErrorStream() << "Failed to create WSCProductList object. Error code: " << hr;
        return false;
    }

    //Code comment by Alex Joseph:
    // Need to log different return codes to differentiate between different types of failures.
    // Also need to log to system wide logger instead of logging to stdout. Assuming a singleton logger 'Logger' exists in the application.
    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
    	Logger::getInstance->getErrorStream() << "Failed to query antivirus product list. Error code: " << hr;
        return false;
    }

	//Code comment by Alex Joseph:
	// Need to log different return codes to differentiate between different types of failures.
	// Also need to log to system wide logger instead of logging to stdout. Assuming a singleton logger 'Logger' exists in the application.
    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
    	Logger::getInstance->getErrorStream() << "Failed to query product count. Error code: " << hr;
        return false;
    }

    //Code comment by Alex Joseph:
    // consider using a meaningful variable name 'productIdx' instead of 'i'.
    for (uint32_t productIdx = 0; productIdx < ProductCount; productIdx++)
    {
		//Code comment by Alex Joseph:
		// Need to log different return codes to differentiate between different types of failures.
		// Also need to log to system wide logger instead of logging to stdout. Assuming a singleton logger 'Logger' exists in the application.
        hr = PtrProductList->get_Item(productIdx, &PtrProduct);
        if (FAILED(hr))
        {
        	Logger::getInstance->getErrorStream() << "Failed to query AV product. Error code: " << hr;
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            Logger::getInstance->getErrorStream() << "Failed to query AV product name. Error code: " << hr;
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        //Code comment by Alex Joseph:
        // Need to free 'PtrVal' immediately after use (SysFreeString)
        SysFreeString(PtrVal);
        PtrVal = nullptr;

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
        	Logger::getInstance->getErrorStream() << "Failed to query AV product state. Error code: " << hr;
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
        	Logger::getInstance->getErrorStream() << "Failed to query AV product definition state. Error code: " << hr;
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
        	Logger::getInstance->getErrorStream() << "Failed to query AV product definition state. Error code: " << hr;
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
