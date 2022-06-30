#include <bits/stdc++.h>

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
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0; // 
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

    hr = CoCreateInstance(		__uuidof(WSCProductList), 
								NULL, 
								CLSCTX_INPROC_SERVER, 
								__uuidof(IWSCProductList), 
								reinterpret_cast<LPVOID*>(&PtrProductList));
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
		// release object in case of error
		if (nullptr != PtrProductList)
		{
			PtrProductList->Release();
		}
        std::cout << "Failed to query product count.";
        return false;
    }

	// ProductCount is a LONG aka a windows typedef to long signed 32 bits data type
	// uint32_t is a unsigned integer
	// typedef unsigned integer type uint32_t
	// the for loop will run within the condition i < ProductCount
	// look what gcc tells me
	/**	
	Compiler executable checksum: 75884ba1d8f1274f120b97d05d513c01
	step2.cpp: In function 'int main()':
	step2.cpp:191:25: warning: comparison of integer expressions of different signedness: 'uint32_t' {aka 'unsigned int'} and 'long int' [-Wsign-compare]
	  191 |  for (uint32_t i = 0; i < ProductCount; i++)
		  |                       ~~^~~~~~~~~~~~~~
	
	A small test 
	long ProductCount2 = 4294967295;
	uint32_t tmp2 = 4294967295;
	
	std::cout << ProductCount2 << std::endl;
	std::cout << tmp2 << std::endl
	
	-1
	4294967295
	
	*/  
	// I would change this for loop from	
    // for (uint32_t i = 0; i < ProductCount; i++)
	// to
	for (LONG i = 0; i < ProductCount; i++)
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
			// need to be release here too
			PtrProduct->Release();
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
			// need to be release here too
			PtrProduct->Release();
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
			// need to be release here too
			PtrProduct->Release();			
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

	if (nullptr != PtrProductList)
    {
        PtrProductList->Release();
    }
	
    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}