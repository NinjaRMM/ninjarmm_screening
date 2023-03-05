#include <wscapi.h>		//WSC API library

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::wstring DefinitionStatus;		// Changed to wstring
    std::wstring Version;
    std::wstring ProductState;
};

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)	// Query for WSC
{
    //Variable declaration and initialization
    HRESULT hr = S_OK;					// COM error code
    IWscProduct* PtrProduct = nullptr;			// Pointer to AV products
    IWSCProductList* PtrProductList = nullptr;		// Pointer to list of AV products
    BSTR PtrVal = nullptr;				// Pointer to a basic string
    LONG ProductCount = 0;				// Number of AV products
    WSC_SECURITY_PRODUCT_STATE ProductState;		// AV state
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;	// AV def. status

    std::wstring displayName, versionNumber, state, timestamp;
    std::wstring definitionState;

    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));	// WSCProductList object instance

    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);	// WSCProductList object initialization, with WSC_SECURITY_PROVIDER_ANTIVIRUS security provider
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }
    hr = PtrProductList->get_Count(&ProductCount);	// AV product count
        if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }
    for (uint32_t i = 0; i < ProductCount; i++)	// iterate through AV products
        {
        hr = PtrProductList->get_Item(i, &PtrProduct);	// AV product pointer
        if (FAILED(hr))
	    {
			std::cout << "Failed to query AV product.";
            continue;
		}
        hr = PtrProduct->get_ProductName(&PtrVal); //Obtain AV name
        if (FAILED(hr))
        {
            PtrProduct->Release();
            std::cout << "Failed to query AV product name.";
            continue;
        }
        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));	// AV product name (basic string to wstring)
        hr = PtrProduct->get_ProductState(&ProductState);	        // AV product state
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product state.";
            continue;
        }
	std::wstring state;	// Determine the AV product state as wstring
	if (ProductState == WSC_SECURITY_PRODUCT_STATE_ON)
	{
		state = L"On";
	} else if (ProductState == WSC_SECURITY_PRODUCT_STATE_OFF)
			{
				state = L"Off";
			}
			else
				{
					state = L"Expired";
				}
	std::wstring definitionStatus;	// AV product definition -> wstring (changed from string to wstring)
	if (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE)
	{
		definitionStatus = "UpToDate";
	}
		else
		{
			definitionStatus = "OutOfDate";
		}
	BSTR versionNumberBSTR;	// AV product version number -> wstring
	hr = PtrProduct->get_ProductVersion(&versionNumberBSTR);
	if (FAILED(hr))
	{
		std::cout << "Failed to get AV product version number." << std::endl;
		continue;
	}
	std::wstring versionNumber = std::wstring(versionNumberBSTR, SysStringLen(versionNumberBSTR));
	SysFreeString(versionNumberBSTR);
	
	BSTR timestampBSTR; // AV product state timestamp -> wstring
	hr = PtrProduct->get_ProductStateTimestamp(&timestampBSTR);
	if (FAILED(hr))
	{
		std::cout << "Failed to get AV product state timestamp." << std::endl;
		continue;
	}
	std::wstring timestamp = std::wstring(timestampBSTR, SysStringLen(timestampBSTR));
	SysFreeString(timestampBSTR);

	ThirdPartyAVSoftware avSoftware;	// Feed struct with the retrieved info
	avSoftware.Name = displayName;
	avSoftware.Description = state;
	avSoftware.DefinitionUpdateTime = timestamp;
	avSoftware.DefinitionStatus = definitionStatus;
	avSoftware.Version = versionNumber;
	avSoftware.ProductState = state;
	thirdPartyAVSoftwareMap[avSoftware.Name] = avSoftware;

	PtrProduct->Release();
	}
		if (thirdPartyAVSoftwareMap.size() == 0)
		{
			return false;
		}
	return true;
	}

int main()
{
	std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap;
	bool success = queryWindowsForAVSoftwareDataWSC(thirdPartyAVSoftwareMap);
	if (success)
		{
		for (const auto& pair : thirdPartyAVSoftwareMap)
			{
			std::wcout << "Name: " << pair.second.Name << std::endl;
			std::wcout << "Description: " << pair.second.Description << std::endl;
			std::wcout << "Version: " << pair.second.Version << std::endl;
			std::wcout << "Definition Update Time: " << pair.second.DefinitionUpdateTime << std::endl;
			std::cout << "Definition Status: " << pair.second.DefinitionStatus << std::endl;
			std::wcout << "Product State: " << pair.second.ProductState << std::endl << std::endl;
			}
		} else
			{
			std::cout << "Failed to retrieve third-party antivirus software information." << std::endl;
			}
	return 0;
}
