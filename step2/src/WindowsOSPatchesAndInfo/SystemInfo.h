/*  File : SystemInfo.h
 *  
 *  Includes the necesary structures and classes to store information from the System and the patched that has been installed on Windows OS
 * 
 *  Compilation may generate some "type cast" warnings for "SystemInfo::QueryWM" Template, please ignore those.
 * 
 * 
 *  usage example:
 * 
 *  	SystemInfoData SIData;
 *
 *		SystemInfo SI;
 * 
 *		SI.GetSystemInfo(SIData);
 *
 *  //-------------   
 * 
 * 		SystemInstalledPatchInfoData SIPData;
 *
 *		SystemInstalledPatchInfo SIPI;
 *
 *		SIPI.GetSystemInstalledPatchInfo(SIPData);
 * 
 * 
 */

#pragma once

#include <map>
#include <vector>
#include <string>
#include <Windows.h>
#include <comdef.h>
#include <Wbemidl.h>

#include<msi.h>
#pragma comment(lib, "msi.lib") 

#include <ntddscsi.h>
#pragma comment(lib, "wbemuuid.lib")

struct SystemProductInfoData
{
	std::vector <std::string> ProductCodes;
	std::string DisplayName;
	std::string LocalPackage;
	std::string MoreInfoURL;
};

struct SystemInstalledPatchInfoData
{
	int NumberOfIntalledPatchesReported;
	std::map<std::string, SystemProductInfoData> Patches;
};

class SystemInstalledPatchInfo
{

private:

	void QueryInstalledPatchInfo(SystemInstalledPatchInfoData& SIPIData)
	{
		DWORD             dwIndex;
		CHAR         szPatchCode[39];
		CHAR         szTargetProductCode[39];
		MSIINSTALLCONTEXT       dwTargetProductContext;
		MSIINSTALLCONTEXT* pdwTargetProductContext = NULL;

		UINT uResult;
		CHAR szBuffer[128];
		DWORD dwSize = 128;

		dwIndex = 0;
		do
		{
			uResult = MsiEnumPatchesExA(NULL, NULL, MSIINSTALLCONTEXT_MACHINE, MSIPATCHSTATE_APPLIED, dwIndex, szPatchCode, szTargetProductCode, (MSIINSTALLCONTEXT*)&dwTargetProductContext, NULL, NULL);// szTargetUserSid, & cchTargetUserSid);
			dwIndex += 1;

			if (uResult == ERROR_SUCCESS)
			{
				SIPIData.Patches[szPatchCode].ProductCodes.push_back(szTargetProductCode);
			}
		} while (uResult == ERROR_SUCCESS);

		SIPIData.NumberOfIntalledPatchesReported = dwIndex - 1;

		for (std::map<std::string, SystemProductInfoData>::iterator itr = SIPIData.Patches.begin(); itr != SIPIData.Patches.end(); ++itr)
		{
			dwSize = 128;
			UINT uInfoResult = MsiGetPatchInfoExA(itr->first.c_str(), itr->second.ProductCodes.front().c_str(), NULL, dwTargetProductContext, "DisplayName", szBuffer, &dwSize);
			itr->second.DisplayName = szBuffer;

			dwSize = 128;
			uInfoResult = MsiGetPatchInfoExA(itr->first.c_str(), itr->second.ProductCodes.front().c_str(), NULL, dwTargetProductContext, "LocalPackage", szBuffer, &dwSize);
			itr->second.LocalPackage = szBuffer;

			dwSize = 128;
			uInfoResult = MsiGetPatchInfoExA(itr->first.c_str(), itr->second.ProductCodes.front().c_str(), NULL, dwTargetProductContext, "MoreInfoURL", szBuffer, &dwSize);
			itr->second.MoreInfoURL = szBuffer;
		}
		return;
	}

public:

	void GetSystemInstalledPatchInfo(SystemInstalledPatchInfoData& SIPIData)
	{
		QueryInstalledPatchInfo(SIPIData);

		return;
	}


	SystemInstalledPatchInfo()
	{
	}

	virtual ~SystemInstalledPatchInfo()
	{
	}
};

struct SystemInfoData
{
	struct CPUObject {

		std::wstring ProcessorId;
		std::wstring Manufacturer;
		std::wstring Name;
		int Cores;
		int Threads;

	}; std::vector <CPUObject> CPU;


	struct GPUObject {

		std::wstring Name;
		std::wstring DriverVersion;
		unsigned long long Memory;
		int XResolution;
		int YResolution;
		int RefreshRate;

	}; std::vector <GPUObject> GPU;


	struct {

		std::wstring Name;
		bool IsHypervisorPresent;
		std::wstring OSVersion;
		std::wstring OSName;
		std::wstring OSArchitecture;
		std::wstring OSSerialNumber;

	} System;


	struct {

		std::wstring ComputerHardwareId;

	} Registry;

};


class SystemInfo
{

public:


	static std::wstring SafeString(const wchar_t* pString)
	{
		return std::wstring((pString == nullptr ? L"(null)" : pString));
	}


	static void RemoveWhitespaces(std::wstring& String)
	{
		String.erase(std::remove(String.begin(), String.end(), L' '), String.end());
	}


private:


	std::wstring GetHKLM(std::wstring SubKey, std::wstring Value)
	{
		DWORD Size;
		std::wstring Ret;

		RegGetValueW(HKEY_LOCAL_MACHINE,
			SubKey.c_str(),
			Value.c_str(),
			RRF_RT_REG_SZ,
			nullptr,
			nullptr,
			&Size);

		Ret.resize(Size);

		RegGetValueW(HKEY_LOCAL_MACHINE,
			SubKey.c_str(),
			Value.c_str(),
			RRF_RT_REG_SZ,
			nullptr,
			&Ret[0],
			&Size);

		return Ret.c_str();
	}


	template <typename T>
	void QueryWMI(std::wstring WMIClass, std::wstring Field, std::vector <T>& Value, const wchar_t* ServerName = L"ROOT\\CIMV2")
	{
		std::wstring Query(L"SELECT ");
		Query.append(Field.c_str()).append(L" FROM ").append(WMIClass.c_str());

		IWbemLocator* Locator;
		IWbemServices* Services;
		IEnumWbemClassObject* Enumerator;
		IWbemClassObject* ClassObject;
		VARIANT Variant;
		DWORD Returned;


		HRESULT hResult{ CoInitializeEx(nullptr, COINIT_MULTITHREADED) };


		if (FAILED(hResult))
		{
			return;
		}


		hResult = CoInitializeSecurity(nullptr,
			-1,
			nullptr,
			nullptr,
			RPC_C_AUTHN_LEVEL_DEFAULT,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			nullptr,
			EOAC_NONE,
			nullptr);



		if (FAILED(hResult))
		{
			CoUninitialize();
			return;
		}


		hResult = CoCreateInstance(CLSID_WbemLocator,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWbemLocator,
			reinterpret_cast<PVOID*>(&Locator));


		if (FAILED(hResult))
		{
			CoUninitialize();
			return;
		}


		hResult = Locator->ConnectServer(_bstr_t(ServerName),
			nullptr,
			nullptr,
			nullptr,
			NULL,
			nullptr,
			nullptr,
			&Services);


		if (FAILED(hResult))
		{
			Locator->Release();
			CoUninitialize();
			return;
		}


		hResult = CoSetProxyBlanket(Services,
			RPC_C_AUTHN_WINNT,
			RPC_C_AUTHZ_NONE,
			nullptr,
			RPC_C_AUTHN_LEVEL_CALL,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			nullptr,
			EOAC_NONE);


		if (FAILED(hResult))
		{
			Services->Release();
			Locator->Release();
			CoUninitialize();
			return;
		}


		hResult = Services->ExecQuery(bstr_t(L"WQL"),
			bstr_t(Query.c_str()),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			nullptr,
			&Enumerator);


		if (FAILED(hResult))
		{
			Services->Release();
			Locator->Release();
			CoUninitialize();
			return;
		}

		while (Enumerator)
		{

			HRESULT Res = Enumerator->Next(WBEM_INFINITE,
				1,
				&ClassObject,
				&Returned);

			if (!Returned) 
			{
				break;
			}

			Res = ClassObject->Get(Field.c_str(),
				0,
				&Variant,
				nullptr,
				nullptr);

			if (typeid(T) == typeid(long) || typeid(T) == typeid(int)) 
			{
				Value.push_back((T)Variant.intVal);
			}
			else if (typeid(T) == typeid(bool)) 
			{
				Value.push_back((T)Variant.boolVal);
			}
			else if (typeid(T) == typeid(unsigned int)) 
			{
				Value.push_back((T)Variant.uintVal);
			}
			else if (typeid(T) == typeid(unsigned short)) 
			{
				Value.push_back((T)Variant.uiVal);
			}
			else if (typeid(T) == typeid(long long)) 
			{
				Value.push_back((T)Variant.llVal);
			}
			else if (typeid(T) == typeid(unsigned long long)) 
			{
				Value.push_back((T)Variant.ullVal);
			}
			else 
			{
				Value.push_back((T)((bstr_t)Variant.bstrVal).copy());
			}

			VariantClear(&Variant);
			ClassObject->Release();
		}

		if (!Value.size())
		{
			Value.resize(1);
		}

		Services->Release();
		Locator->Release();
		Enumerator->Release();
		CoUninitialize();
	}

	void QueryProcessor(SystemInfoData& SIData)
	{
		std::vector <const wchar_t*> ProcessorId;
		std::vector <const wchar_t*> Manufacturer;
		std::vector <const wchar_t*> Name;
		std::vector <int> Cores;
		std::vector <int> Threads;

		QueryWMI<const wchar_t*>(L"Win32_Processor", L"ProcessorId", ProcessorId);
		QueryWMI<const wchar_t*>(L"Win32_Processor", L"Manufacturer", Manufacturer);
		QueryWMI<const wchar_t*>(L"Win32_Processor", L"Name", Name);
		QueryWMI<int>(L"Win32_Processor", L"NumberOfCores", Cores);
		QueryWMI<int>(L"Win32_Processor", L"NumberOfLogicalProcessors", Threads);

		SIData.CPU.resize(Name.size());

		for (int i = 0; i < Name.size(); i++)
		{
			SIData.CPU.at(i).Name = SafeString(Name.at(i));
			SIData.CPU.at(i).ProcessorId = SafeString(ProcessorId.at(i));
			SIData.CPU.at(i).Manufacturer = SafeString(Manufacturer.at(i));
			SIData.CPU.at(i).Cores = Cores.at(i);
			SIData.CPU.at(i).Threads = Threads.at(i);
		}
	}


	void QueryGPU(SystemInfoData& SIData)
	{
		std::vector <const wchar_t*> Name;
		std::vector <const wchar_t*> DriverVersion;
		std::vector <unsigned long long> Memory;
		std::vector <int> XRes;
		std::vector <int> YRes;
		std::vector <int> RefreshRate;

		QueryWMI(L"Win32_VideoController", L"Name", Name);
		QueryWMI(L"Win32_VideoController", L"DriverVersion", DriverVersion);
		QueryWMI(L"Win32_VideoController", L"AdapterRam", Memory);
		QueryWMI(L"Win32_VideoController", L"CurrentHorizontalResolution", XRes);
		QueryWMI(L"Win32_VideoController", L"CurrentVerticalResolution", YRes);
		QueryWMI(L"Win32_VideoController", L"CurrentRefreshRate", RefreshRate);

		SIData.GPU.resize(Name.size());

		for (int i = 0; i < Name.size(); i++) 
		{
			SIData.GPU.at(i).Name = SafeString(Name.at(i));
			SIData.GPU.at(i).DriverVersion = SafeString(DriverVersion.at(i));
			SIData.GPU.at(i).Memory = (Memory.at(i) * 2 / (1024 * 1024) / 1000);
			SIData.GPU.at(i).XResolution = XRes.at(i);
			SIData.GPU.at(i).YResolution = YRes.at(i);
			SIData.GPU.at(i).RefreshRate = RefreshRate.at(i);
		}
	}


	void QuerySystem(SystemInfoData& SIData)
	{
		std::vector <const wchar_t*> SystemName;
		std::vector <const wchar_t*> OSVersion;
		std::vector <const wchar_t*> OSName;
		std::vector <const wchar_t*> OSArchitecture;
		std::vector <const wchar_t*> OSSerialNumber;
		std::vector <bool> IsHypervisorPresent;

		QueryWMI(L"Win32_ComputerSystem", L"Name", SystemName);
		QueryWMI(L"Win32_ComputerSystem", L"Model", IsHypervisorPresent);
		QueryWMI(L"Win32_OperatingSystem", L"Version", OSVersion);
		QueryWMI(L"Win32_OperatingSystem", L"Name", OSName);
		QueryWMI(L"Win32_OperatingSystem", L"OSArchitecture", OSArchitecture);
		QueryWMI(L"Win32_OperatingSystem", L"SerialNumber", OSSerialNumber);

		std::wstring wOSName{ SafeString(OSName.at(0)) };

		if (wOSName.find('|') != std::wstring::npos) 
		{
			wOSName.resize(wOSName.find('|'));
		}

		SIData.System.Name = SafeString(SystemName.at(0));
		SIData.System.IsHypervisorPresent = IsHypervisorPresent.at(0);
		SIData.System.OSName = wOSName;
		SIData.System.OSVersion = SafeString(OSVersion.at(0));
		SIData.System.OSSerialNumber = SafeString(OSSerialNumber.at(0));
		SIData.System.OSArchitecture = SafeString(OSArchitecture.at(0));
	}


	void QueryRegistry(SystemInfoData& SIData)
	{
		SIData.Registry.ComputerHardwareId = SafeString(GetHKLM(L"SYSTEM\\CurrentControlSet\\Control\\SystemInformation", L"ComputerHardwareId").c_str());
	}


public:


	void GetSystemInfo(SystemInfoData& SIData)
	{
		QueryProcessor(SIData);
		QueryGPU(SIData);
		QuerySystem(SIData);
		QueryRegistry(SIData);

		return;
	}


	SystemInfo()
	{
	}

	virtual ~SystemInfo()
	{
	}
};
