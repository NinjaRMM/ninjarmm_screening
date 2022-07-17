#define NTDDI_VERSION NTDDI_WIN7
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include "sdkddkver.h"
#include <iostream>
#include <ShlObj.h>
#include <atlbase.h>

int main()
{
	// Query for installed windows patches 
#ifdef _WIN32
	HRESULT hr = CoInitialize(NULL);
	int count = 0;

	if (SUCCEEDED(hr))
	{
		CComPtr<IShellItem> pUpdates;
		CComPtr<IEnumShellItems> pShellEnum;

		hr = SHGetKnownFolderItem(FOLDERID_AppUpdates, static_cast<KNOWN_FOLDER_FLAG>(0), nullptr, IID_PPV_ARGS(&pUpdates));
		hr = pUpdates->BindToHandler(nullptr, BHID_EnumItems, IID_PPV_ARGS(&pShellEnum));
		if (pShellEnum)
		{
			do 
			{
				CComPtr<IShellItem> pItem;
				CComHeapPtr<WCHAR> szName;

				hr = pShellEnum->Next(1, &pItem, nullptr);
				if (pItem)
				{
					HRESULT hres = pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &szName);
					std::cout << static_cast<LPWSTR>(szName) << std::endl;
					count++;
				}
			} while (hr == S_OK);
		}
	}
	CoUninitialize();
	std::cout << L"Found " << count << " updates" << std::endl;

	// Query for installed Linux Packages
#elif __linux__
	if (system("echo -n 'Installed Linux packages '; sudo apt list --installed | less") == 0)
	{
		std::cout << "Installed Linux packages successfully listed" << std::endl;
	}
	else
	{
		std::cout << "Failed to get the list os Linux installed packages" << std::endl;
	}

#elif __APPLE__ 
	if (system("pkgutil --pkgs") == 0)
	{
		std::cout << "Installed Mac packages successfully listed" << std::endl;
	}
	else
	{
		std::cout << "Failed to get the list os Mac installed packages" << std::endl;
	}
#endif
	return 0;
}