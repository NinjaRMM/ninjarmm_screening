/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

#include "CodeReview.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif


struct ThirdPartyAVSoftware
{
	std::wstring Name;
	std::wstring Description;
	std::wstring DefinitionUpdateTime;
	std::string DefinitionStatus;
	std::wstring Version;
	std::wstring ProductState;
};

template<typename T>
class DeleteHelper
{
public:
	DeleteHelper() = delete;
	DeleteHelper(T* ptr) : _ptr(ptr)
	{}
	~DeleteHelper()
	{
		if (_ptr)
			_ptr->Release();
	}
private:
	T* _ptr;


};

class DeleteOleStringHelper
{
public:
	DeleteOleStringHelper() = delete;
	DeleteOleStringHelper(wchar_t* ptr) : _ptr(ptr)
	{}
	~DeleteOleStringHelper()
	{
		if (_ptr)
			SysFreeString(_ptr);

	}
private:
	wchar_t* _ptr;


};



bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
	HRESULT hr = S_OK;
	IWscProduct* PtrProduct = nullptr;

	IWSCProductList* PtrProductList = nullptr;
	BSTR PtrVal1 = nullptr;
	BSTR PtrVal2 = nullptr;

	LONG ProductCount = 0;
	WSC_SECURITY_PRODUCT_STATE ProductState;
	WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

	std::wstring displayName, versionNumber, state, timestamp;
	std::string definitionState;

	hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
	DeleteHelper dpl(PtrProductList);
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
		DeleteHelper dpl(PtrProduct);

		if (FAILED(hr))
		{
			std::cout << "Failed to query AV product.";
			continue;
		}
		
		hr = PtrProduct->get_ProductName(&PtrVal1);
		DeleteOleStringHelper pname(PtrVal1);
		if (FAILED(hr))
		{
			//PtrProduct->Release();
			std::cout << "Failed to query AV product name.";
			continue;
		}

		displayName = std::wstring(PtrVal1, SysStringLen(PtrVal1));

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

		hr = PtrProduct->get_ProductStateTimestamp(&PtrVal2);
		DeleteOleStringHelper ptimestamp(PtrVal2);

		if (FAILED(hr))
		{
			std::cout << "Failed to query AV product definition state.";
			continue;
		}
		timestamp = std::wstring(PtrVal2, SysStringLen(PtrVal2));
		//SysFreeString(PtrVal);

		ThirdPartyAVSoftware thirdPartyAVSoftware;
		thirdPartyAVSoftware.Name = std::move(displayName);
		thirdPartyAVSoftware.DefinitionStatus = std::move(definitionState);
		thirdPartyAVSoftware.DefinitionUpdateTime = std::move(timestamp);
		thirdPartyAVSoftware.Description = std::move(state);
		thirdPartyAVSoftware.ProductState = std::move(state);
		thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = std::move( thirdPartyAVSoftware);

		//PtrProduct->Release();
	}

	// PtrProductList->Release();

	if (thirdPartyAVSoftwareMap.size() == 0)
	{
		return false;
	}
	return true;
}


void test()
{

	CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftware;

	if (queryWindowsForAVSoftwareDataWSC(thirdPartyAVSoftware))
	{
		for (auto const& [key, val] : thirdPartyAVSoftware)
		{
			std::wcout << key << L":" << std::endl;
			std::wcout << L"\t" << L"Name" << L": " << val.Name << std::endl;
			std::cout << "\t" << "DefinitionStatus" << ": " << val.DefinitionStatus << std::endl;
			std::wcout << L"\t" << L"DefinitionUpdateTime" << L": " << val.DefinitionUpdateTime << std::endl;
			std::wcout << L"\t" << L"Description" << L": " << val.Description << std::endl;
			std::wcout << L"\t" << L"ProductState" << L": " << val.ProductState << std::endl;
			std::wcout << L"\t" << L"Version" << ": " << val.Version << std::endl;

			std::wcout << "______" << std::endl;
		}
	}

	CoUninitialize();

}

int main()
{

	test();
	_CrtDumpMemoryLeaks();

	return 0;
}