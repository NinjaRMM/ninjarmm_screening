// SystemInfo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define NTDDI_VERSION NTDDI_WIN7
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include "sdkddkver.h"
#include <iostream>
#include <ShlObj.h>
#include <atlbase.h>
using namespace std;

// **** NOTE Windows only implementation

int GetUpdatesFolder()
{
	HRESULT hr = CoInitialize(NULL);

	if (SUCCEEDED(hr))
	{
		CComPtr<IShellItem> pUpdates;
		CComPtr<IEnumShellItems> pShellEnum;

		// The virtual folder FOLDERID_AppUpdates contains the list of updates
		hr = SHGetKnownFolderItem(FOLDERID_AppUpdates, static_cast<KNOWN_FOLDER_FLAG>(0), nullptr, IID_PPV_ARGS(&pUpdates));
		hr = pUpdates->BindToHandler(nullptr, BHID_EnumItems, IID_PPV_ARGS(&pShellEnum));
		if (pShellEnum)
		{
			while (hr == S_OK)
			{
				CComPtr<IShellItem> pItem;
				CComHeapPtr<WCHAR> szName;

				// Get the next shell item
				hr = pShellEnum->Next(1, &pItem, nullptr);
				if (pItem)
				{
					HRESULT hres = pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &szName);
					// Is wide character
					wcout << static_cast<LPWSTR>(szName) << endl;
				}
			}
		}
	}
	CoUninitialize();
	return 0;
}


void GetSystemInformation()
{
	SYSTEM_INFO siSysInfo;

	// Copy the hardware information to the SYSTEM_INFO structure. 
	GetSystemInfo(&siSysInfo);

	// Display the contents of the SYSTEM_INFO structure. 
	cout << endl << endl;
	cout << "Hardware information:" << endl;
	cout << "OEM ID: " << siSysInfo.dwOemId << endl;
	cout << "Number of processors: " << siSysInfo.dwNumberOfProcessors << endl;
	cout << "Processor type: " << siSysInfo.dwProcessorType << endl;

	cout << "Page size: " << siSysInfo.dwPageSize << endl;
	cout << "Minimum application address: " << siSysInfo.lpMinimumApplicationAddress << endl;
	cout << "Maximum application address: " << siSysInfo.lpMaximumApplicationAddress << endl;
	cout << "Active processor mask: " << siSysInfo.dwActiveProcessorMask << endl;

	return;
}

int main()
{
	if (GetUpdatesFolder())
	{
		cout << "Failed to get updates." << endl;
		return -1;
	}

	GetSystemInformation();

	return 0;
}
