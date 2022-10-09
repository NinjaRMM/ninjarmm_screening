// required includes
/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/


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
    HRESULT hr = S_OK;  // auto? redundant definition, type deducted from right side
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    // split definition and move it just the line before first use, the code is so large and
    // the jump for the readibility is so big. Also for the compiler is better define in the above line. 
    std::wstring displayName, versionNumber, state, timestamp;    // version number is not used in code below.!!
    std::string definitionState;

    // nullptr instead of NULL macro
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))  // multiple return, I would prefere refactor the code having only one exit point.
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;  // I'm used to work with 0 when OK, 1 when NO OK
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))  //multiple return, I would prefere refactor the code having only one exit point.
    {
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))  // multiple return, I would prefere refactor the code having only one exit point.
    {
        std::cout << "Failed to query product count.";
        return false;
    }


	// I miss some checks if the pointers are created
	// probably at the end of the file with an unique return statement
	//
	// if (nullptr != PtrVal)
    // {
    //    SysFreeString(PtrVal);
    // }
    // if (nullptr != PtrProductList)
    // {
    //    PtrProductList->Release();
    // }
    // if (nullptr != PtrProduct)
    // {
    //    PtrProduct->Release();
    // }


    for (uint32_t i = 0; i < ProductCount; i++)  // LONG?? comparison between signed/unsigned
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))  // Multiple continue statements, when the code is large the readibility gets worse. I would prefer if else statements and indent code.
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))  // Multiple continue statements, when the code is large the readibility gets worse. I would prefer if else statements and indent code.
        {
            PtrProduct->Release();
            std::cout << "Failed to query AV product name.";
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))   // Multiple continue statements, when the code is large the readibility gets worse. I would prefer if else statements and indent code.
        {
            std::cout << "Failed to query AV product state.";  // Memory leak if PtrProduct is not Released
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
        if (FAILED(hr))   // Multiple continue statements, when the code is large the readibility gets worse. I would prefer if else statements and indent code.
        {
            std::cout << "Failed to query AV product definition state.";   // Memory leak if PtrProduct is not Released
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))   // Multiple continue statements, when the code is large the readibility gets worse. I would prefer if else statements and indent code.
        {
            std::cout << "Failed to query AV product definition state.";    // Memory leak if PtrProduct is not Released
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);
		// PtrVal = nullptr; Memory leak????

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;  // versionNumber rather than state??
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    if (thirdPartyAVSoftwareMap.size() == 0)  // instead of size() == 0, empty is created for that purpose
    {
        return false;
    }
    return true;
}