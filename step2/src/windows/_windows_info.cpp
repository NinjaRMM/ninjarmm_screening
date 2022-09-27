#include "_windows_info.h"
#include "_windows_helpers.h"
#include "system_info.h"
#include <Windows.h>
#include <tchar.h>
#include <intrin.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <WMIService.h>
///#include <OAIdl.h>
using namespace std;

std::wstring get_windows_total_memory(
	const vector<LPCWSTR>& queryAttrs);


wchar_t const* RAMFormFactors[24]{
	L"Unknown form factor",
	L"",
	L"SIP",
	L"DIP",
	L"ZIP",
	L"SOJ",
	L"Proprietary",
	L"SIMM",
	L"DIMM",
	L"TSOP",
	L"PGA",
	L"RIMM",
	L"SODIMM",
	L"SRIMM",
	L"SMD",
	L"SSMP",
	L"QFP",
	L"TQFP",
	L"SOIC",
	L"LCC",
	L"PLCC",
	L"BGA",
	L"FPBGA",
	L"LGA"
};

wchar_t const* RAMMemoryTypes[] = {
	L"Unknown",
	L"Other",
	L"DRAM",
	L"Synchronous DRAM",
	L"Cache DRAM",
	L"EDO",
	L"EDRAM",
	L"VRAM",
	L"SRAM",
	L"RAM",
	L"ROM",
	L"Flash",
	L"EEPROM",
	L"FEPROM",
	L"EPROM",
	L"CDRAM",
	L"3DRAM",
	L"SDRAM",
	L"SGRAM",
	L"RDRAM",
	L"DDR",
	L"DDR2",
	L"DDR2 FB-DIMM",
	L"",
	L"DDR3",
	L"FBD2",
	L"DDR4",
	L"LPDDR",
	L"LPDDR2",
	L"LPDDR3",
	L"LPDDR4",
	L"Logical non-volatile device",
	L"HBM",
	L"HBM2"
};




std::wstring get_windows_os() 
{
	vector<LPCWSTR> queryAttrs = { L"Name", L"OSArchitecture" };

	IEnumWbemClassObject* pEnumerator = WMIService::GetInstance().ExecuteWQLQuery(L"Win32_OperatingSystem", queryAttrs);
	DeleteHelper<IEnumWbemClassObject> enumerator(pEnumerator);
	wstring os = L"";


	IWbemClassObject* pclsObj = nullptr;
	ULONG uReturn = 0;

	while (pEnumerator) {
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn) {
			break;
		}
		if (FAILED(hr))
			throw std::exception("IEnumWbemClassObject::Next failure.");

		DeleteHelper<IWbemClassObject> obj(pclsObj);

		VARIANT vtProp;
		hr = pclsObj->Get(queryAttrs[(int)WMI_OS::NAME], 0, &vtProp, 0, 0);
		wstring fullOSstring;
		string OSName;
		wstring OSArchitecture;
		wstring OSNameWide;

		OSNameWide = vtProp.bstrVal;
		VariantClear(&vtProp);
		hr = pclsObj->Get(queryAttrs.at((int)WMI_OS::ARCHITECTURE), 0, &vtProp, 0, 0);
		OSArchitecture = vtProp.bstrVal;
		VariantClear(&vtProp);

		int garbageIndex = OSNameWide.find(L"|");
		OSNameWide = OSNameWide.erase(garbageIndex, OSNameWide.length() - garbageIndex);
		fullOSstring = OSNameWide + L" " + OSArchitecture;

		os=fullOSstring;
	}
	return os;
}

std::wstring get_windows_total_memory(
	const vector<LPCWSTR>& queryAttrs) {


	IEnumWbemClassObject* pEnumerator = WMIService::GetInstance().ExecuteWQLQuery(L"Win32_PhysicalMemory", queryAttrs);
	DeleteHelper<IEnumWbemClassObject> enumerator(pEnumerator);


	IWbemClassObject* pclsObj = nullptr;
	ULONG uReturn = 0;

	double accumulatedRAM = 0;
	while (pEnumerator) {
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn) {
			break; //no more data
		}
		if (FAILED(hr))
			throw std::exception("IEnumWbemClassObject::Next failure.");
		VARIANT vtProp;
		hr = pclsObj->Get(queryAttrs[((int)WMI_RAM::CAPACITY)], 0, &vtProp, 0, 0);
		DeleteVariantHelper vaD(&vtProp);
		double cap;

		wstring temp;
		TCHAR tempChar[100];
		temp = vtProp.bstrVal;
		_tcscpy(tempChar, temp.c_str());
		swscanf(tempChar, L"%lf", &cap);

		cap /= (pow(1024, 3));
		accumulatedRAM += cap;
	}
	TCHAR capacityStrBuff[100];
	_stprintf(capacityStrBuff, _T("%.2lf"), accumulatedRAM);
	return wstring(capacityStrBuff);
}

std::wstring get_windows_RAM() {

	std::vector<LPCWSTR> queryAttrs = { L"Capacity", L"FormFactor", L"MemoryType", L"Speed" };
	IEnumWbemClassObject* pEnumerator = WMIService::GetInstance().ExecuteWQLQuery(L"Win32_PhysicalMemory", queryAttrs);
	DeleteHelper<IEnumWbemClassObject> enumerator(pEnumerator);
	wstring ram = L"";
	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator) {
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn) {
			break;
		}
		DeleteHelper<IWbemClassObject> enumerator(pclsObj);

		VARIANT vtProp;

		// format: gb channel ddr @ mhz (no timings yet)

		wstring clockStr;
		UINT32 clock;
		UINT16 formFactor;
		wstring name;
		wstring capacityStr;
		wstring formFactorStr;
		wstring memoryTypeStr;
		UINT16 memoryType;
		TCHAR clockStrBuff[10];

		capacityStr = get_windows_total_memory( queryAttrs);
		
		hr = pclsObj->Get(queryAttrs[(int)WMI_RAM::FORMFACTOR], 0, &vtProp, 0, 0);
		formFactor = vtProp.uintVal;
		VariantClear(&vtProp); // redundant but for safety

		formFactorStr = RAMFormFactors[formFactor];

		hr = pclsObj->Get(queryAttrs[(int)WMI_RAM::MEMTYPE], 0, &vtProp, 0, 0);
		memoryType = vtProp.uintVal;
		VariantClear(&vtProp); // redundant but for safety

		memoryTypeStr = RAMMemoryTypes[memoryType];

		hr = pclsObj->Get(queryAttrs[(int)WMI_RAM::SPEED], 0, &vtProp, 0, 0);
		clock = vtProp.uintVal;
		VariantClear(&vtProp); // redundant but for safety

		_stprintf(clockStrBuff, _T("%d"), clock);
		clockStr = wstring(clockStrBuff);
		ram=
			capacityStr +
			L" GB " + formFactorStr + L" " +
			(memoryTypeStr.length() == 0 ? L"" : memoryTypeStr + L" ") +
			clockStr + L"MHz";

	}
	return ram;
}


std::wstring get_windows_CPU() {

	std::vector<LPCWSTR> queryAttrs = { { L"Name" } };//// , L"MaxClockSpeed", L"SocketDesignation"

	IEnumWbemClassObject* pEnumerator = WMIService::GetInstance().ExecuteWQLQuery(L"Win32_Processor", queryAttrs);
	DeleteHelper<IEnumWbemClassObject> enumerator(pEnumerator);

	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;
	wstring processor=L"";
	while (pEnumerator) {
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn) {
			break; //No more data
		}
		if (FAILED(hr))
			throw std::exception("IEnumWbemClassObject::Next failure.");

		DeleteHelper<IWbemClassObject> obj(pclsObj);
		VARIANT vtProp;

		hr = pclsObj->Get(queryAttrs[(int)WMI_CPU::NAME], 0, &vtProp, 0, 0);
		DeleteVariantHelper vaD(&vtProp);
		
		processor = vtProp.bstrVal;

	}

	return processor;
}

// this module is private for windows. I create a project with Code::clocks for linux. With cmake we need add this module only in linux.

bool system_get_info(SystemInfo &info)
{

	info.cpu = utf8_encode( get_windows_CPU().c_str());
	info.memory = utf8_encode(get_windows_RAM().c_str());
	info.os = utf8_encode(get_windows_os().c_str());


    return true;

}
