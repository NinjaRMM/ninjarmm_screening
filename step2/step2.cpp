// file header and copyright here
//*******************************************************************
//detect os
#if defined(__WINDOWS__) || defined(__CYGWIN__) || defined(_WIN32) || defined(WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WIN64)
	#define PLAT_OS_WIN
#elif defined(__ANDROID__) || defined(ANDROID) || defined(_ANDROID)
	#define PLAT_OS_ANDROID
#elif defined(__gnu_linux__) || defined(__linux__) || defined(linux) || defined(__linux) || defined(_linux)
	#define PLAT_OS_LINUX
#elif defined(macintosh) || defined(Macintosh)
	#define PLAT_OS_MACOS9
#elif defined(__APPLE__) && defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#define PLAT_OS_IOS
	#elif TARGET_OS_IPHONE == 1
		#define PLAT_OS_IOS
	#elif TARGET_OS_MAC == 1
		#define PLAT_OS_MACOSX
	#else
		#define PLAT_OS_MACOSX	//default to osx
	#endif
#elif defined(__unix__) || defined(__unix) || defined(_unix)
	#define PLAT_OS_UNIX
#endif
//*******************************************************************
//specific os headers and defs
#if defined(PLAT_OS_WIN)
	#define NTDDI_VERSION NTDDI_WIN7
	#define _WIN32_WINNT _WIN32_WINNT_WIN7
	#include "sdkddkver.h"
	#include <iostream>
	#include <ShlObj.h>
	#include <atlbase.h>
	using namespace std;
#endif

#include <stdlib.h>
#include <string>
//*******************************************************************
int main()
{
	//Compiled on windows with vs2019, on linux w/ codeblocks/gcc.
	//Confirmed compilation and execution on win7/64bit and linux/ubuntu/64bit, don't have a mac to test/build on.
	//I did manual validation of the output, mostly because I was either outputting to console or 
	//running a console app which output to console. Only the win shell code directly handles the strings.
	//There doesn't seem to be a standard cross-linux method of getting the information.
	//I would preferably find API calls to get all the info. Otherwise, I would either send the output to file or 
	//re-direct the child console app's output to my parent app's input and parse the input into key-value pairs 
	//and put into a map. Having all the info in key-value pairs would allow for useful unit testing of the output.

	#if defined(PLAT_OS_WIN)	//tested on win7 64bit

	//shell code below cribbed from microsoft's forum with modifications
	HRESULT hr = CoInitialize(NULL);
	int count = 0;

	char ch = 0;

	if (SUCCEEDED(hr))
	{
		CComPtr<IShellItem> pUpdates;
		CComPtr<IEnumShellItems> pShellEnum;

		//note: seems to need win64
		hr = SHGetKnownFolderItem(FOLDERID_AppUpdates, static_cast<KNOWN_FOLDER_FLAG>(0), nullptr, IID_PPV_ARGS(&pUpdates));
		hr = pUpdates->BindToHandler(nullptr, BHID_EnumItems, IID_PPV_ARGS(&pShellEnum));
		if (pShellEnum)
		{
			do {
				CComPtr<IShellItem> pItem;
				CComHeapPtr<WCHAR> szName;

				hr = pShellEnum->Next(1, &pItem, nullptr);
				if (pItem)
				{
					HRESULT hres = pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &szName);
					wcout << static_cast<LPWSTR>(szName) << endl;
					if (count && !(count % 20))	//pause every 20 lines
					{
						cout << "Press enter to continue" << endl; //console was scrolling past back buffer size, couldn't scroll to top
						cin.get(ch);
					}
					count++;
				}
			} while (hr == S_OK);
		}
	}
	CoUninitialize();
	wcout << L"Found " << count << " updates" << endl;

	cout << "Press enter to continue" << endl;	//console was scrolling past back buffer size, couldn't scroll to top
	//char ch = 0;
	cin.get(ch);

	::system("systeminfo");

	//console should stay open and prompt for key press to close here

	#elif defined(PLAT_OS_LINUX) || defined(PLAT_OS_ANDROID)	//tested on ubuntu

	std::string s = "apt list --installed; "
					"lshw -short; "
					"lscpu; "
					"lsblk -a; "
					"lsusb; "
					"lspci; "
					"lspci -t; "
					"exec bash";
	::system(s.c_str());

	#elif defined(PLAT_OS_MACOSX) || defined(PLAT_OS_MACOS9)	//not tested

	::system("softwareupdate --history --all; exec bash");

	#endif

	return 0;
}
//*******************************************************************
