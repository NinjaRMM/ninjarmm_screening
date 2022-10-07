/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/
#include <atlbase.h>

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
    
    /** if you havent already done this ... initialize COM */
    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
      std::cout << "Failed to initialize COM.";
      return false;
    }

    /** I moved all the variables down close to where they were used */

    /** Use smart pointers to deal with all the returns and continues without leaking ref counts (and objects) */
    CComPtr<IWSCProductList> PtrProductList;
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), &PtrProductList);
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

    LONG ProductCount = 0;
    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }
    
    /** 
     * so, we do not know the stae of the map on input so, i am assuming that this method will get all 
     * the products. clear the map 
     */
    thirdPartyAVSoftwareMap.clear();  
    
    /** Changed to LONG to avoid type mismatch and possible compiler warning */
    for (LONG i = 0; i < ProductCount; i++)
    {
        /** use smart ptr */
        CComPtr<IWscProduct> PtrProduct;
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }
        
        /** use smart BSTR wrapper, dont wanna leak a bstr now when we continue or return */
        CComBSTR PtrVal;
        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product name.";
            continue;
        }
        std::wstring displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

	// added a default to Unknown they may add a state in future who knows
	std::wstring state("Unknown");
        WSC_SECURITY_PRODUCT_STATE ProductState;
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
        /** you missed a state */
        else if (ProductState == WSC_SECURITY_PRODUCT_STATE_SNOOZED)
        {
            state = L"Snoozed";
        }
        else if (ProductState == WSC_SECURITY_PRODUCT_STATE_EXPIRED)
        {
            state = L"Expired";
        }

        WSC_SECURITY_SIGNATURE_STATUS ProductStatus;
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            /** fixed error message, copy and paste kills peaches */
            std::cout << "Failed to query AV product signature status.";
            continue;
        }
        std::string definitionStatus = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            /** fixed error message, copy and paste kills peaches */        
            std::cout << "Failed to query AV product state timestamp.";
            continue;
        }
        std::wstring timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
 
        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionStatus;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        /** 
         * display name is probably better as a description instead of state .... 
         * if you need a description .... the object doesnt expose a method to get one so ....
         * you should probably remove the description if you cannot get a meaningful one 
         * or i guess you could settle on display name 
         */
        thirdPartyAVSoftware.Description = displayName; 
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
                
        /** Removed the release WAY down here since with all those continues, you may not get here 
            and we are now using smart pointers */
   }

   /** removed the if logic since map knows if its empty */
   return !thirdPartyAVSoftwareMap.empty();
}
