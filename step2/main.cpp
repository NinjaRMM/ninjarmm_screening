#include "CodeReview.h"

#include <iostream>
#include <Windows.h>
#include <comdef.h>
#include <winerror.h>

int main ()
{
	HRESULT hr = S_OK;
	hr = CoInitialize(nullptr); // not going to assume system initialized
	if (FAILED(hr))
	{
		_com_error err(hr);
		std::cerr << "Failed to initialize COM library. Error " << std::hex << hr << " " << err.ErrorMessage(); //changed to std::cerr and added return code
		return false;
	}

	std::map<std::wstring, ThirdPartyAVSoftware> tpavsMap;
	auto result = queryWindowsForAVSoftwareDataWSC(tpavsMap);
	for (const auto& i : tpavsMap)
	{
		std::wcout << L"Map name: " << i.first << std::endl;
		std::wcout << L"Name: " << i.second.Name << std::endl;
		std::wcout << L"Version: " << i.second.Version << std::endl;
		std::wcout << L"Description: " << i.second.Description << std::endl;
		std::wcout << L"DefinitionStatus: " << i.second.DefinitionStatus << std::endl;
		std::wcout << L"DefinitionUpdateTime: " << i.second.DefinitionUpdateTime << std::endl;
		std::wcout << L"ProductState: " << i.second.ProductState << std::endl;
		std::wcout << std::endl;
	}
	return 0;
}