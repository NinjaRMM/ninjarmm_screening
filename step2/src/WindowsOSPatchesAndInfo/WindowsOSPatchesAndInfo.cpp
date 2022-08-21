/* Developed and tested on :
 * Microsoft Visual Studio Community 2022
 * Version 17.3.1
 * VisualStudio.17.Release / 17.3.1 + 32811.315
 * Microsoft.NET Framework
 * Version 4.8.04084
 *
 * Installed Version : Community
 *
 * Visual C++ 2022   00482 - 90000 - 00000 - AA311
 * Microsoft Visual C++ 2022
 *
 * WindowsOSPatchesAndInfo.cpp : This file contains the 'main' function. Program execution begins and ends there.
 */

/*
 * This code is ONLY to generate a program that runs on windows OS, it will not run on linux nor Mac
 * I validate results checking against windows control panel information and windows system registery
 * to use it in other project / production, use "SystemInfo.h",
 * include that file in your project and use it the way is shown in the main function
 */

#include <map>
#include <iostream>
#include <vector>
#include <string>

#include "SystemInfo.h"
// *  Compilation may generate some "type cast" warnings for "SystemInfo::QueryWM" Template, please ignore those.

int main()
{
	SystemInfoData SIData;

	SystemInfo SI;

	// get System Information

	SI.GetSystemInfo(SIData);

	// print System Information

	std::cout << '\n';

	std::wcout << L"System Name:\t\t" << SIData.System.Name << std::endl;
	std::wcout << L"Hypervisor Present:\t" << (SIData.System.IsHypervisorPresent ? L"Yes" : L"No") << std::endl;
	std::wcout << L"OS Title:\t\t" << SIData.System.OSName << std::endl;
	std::wcout << L"OS Version:\t\t" << SIData.System.OSVersion << std::endl;
	std::wcout << L"OS Architecture:\t" << SIData.System.OSArchitecture << std::endl;
	std::wcout << L"OS Serial Number:\t" << SIData.System.OSSerialNumber << std::endl;

	std::cout << '\n';

	std::wcout << L"Computer Hardware Id:\t" << SIData.Registry.ComputerHardwareId << std::endl;
	
	std::cout << '\n';

	for (int i = 0; i < SIData.CPU.size(); i++)
	{
		std::wcout << L"CPU #" << i + 1 << std::endl;
		std::wcout << L"Name:\t\t\t" << SIData.CPU.at(i).Name << std::endl;
		std::wcout << L"Processor Id:\t\t" << SIData.CPU.at(i).ProcessorId << std::endl;
		std::wcout << L"Manufacturer:\t\t" << SIData.CPU.at(i).Manufacturer << std::endl;
		std::wcout << L"Cores:\t\t\t" << SIData.CPU.at(i).Cores << std::endl;
		std::wcout << L"Threads Rate:\t\t" << SIData.CPU.at(i).Threads << std::endl;
	}

	std::cout << '\n';

	for (int i = 0; i < SIData.GPU.size(); i ++)
	{
		std::wcout << L"GPU #" << i + 1 << std::endl;
		std::wcout << L"Name:\t\t\t" << SIData.GPU.at(i).Name << std::endl;
		std::wcout << L"Driver Version:\t\t" << SIData.GPU.at(i).DriverVersion << std::endl;
		std::wcout << L"Memory:\t\t\t" << SIData.GPU.at(i).Memory << L" GB" << std::endl;
		std::wcout << L"Resolution:\t\t" << SIData.GPU.at(i).XResolution << L"x" << SIData.GPU.at(i).YResolution << std::endl;
		std::wcout << L"Refresh Rate:\t\t" << SIData.GPU.at(i).RefreshRate << std::endl;
	}

	std::cout << '\n';

	SystemInstalledPatchInfoData SIPData;

	SystemInstalledPatchInfo SIPI;

	// get Installed Patches Information

	SIPI.GetSystemInstalledPatchInfo(SIPData);

	// print Installed Patches Information

	std::cout << "\nInstalled Patches found: " << SIPData.Patches.size() << "\n";
    std::cout << "Number of Products where they where applied on: " << SIPData.NumberOfIntalledPatchesReported << "\n\n"; 
	
	std::cout << '\n' << "Press ENTER to show Installed Patches Details....";
	std::cin.get();
	std::cout << '\n';

	for (std::map<std::string, SystemProductInfoData>::iterator itr = SIPData.Patches.begin(); itr != SIPData.Patches.end(); ++itr)
    {
        std::cout << "Patch Code: " << itr->first << "\n";
        std::cout << "\tDisplay Name: " << itr->second.DisplayName << "\n";
        std::cout << "\tLocal Package: " << itr->second.LocalPackage << "\n";
        std::cout << "\tMore Info URL: " << itr->second.MoreInfoURL << "\n";
        std::cout << "\tApplied on products:\n";

        for (std::string s : itr->second.ProductCodes)
        {
            std::cout << "\t\t" << s << "\n";
        }

        std::cout << "\n\n";

    }

    std::cout << '\n' << "Press ENTER to finish...";
    std::cin.get();

    return 0;
}
