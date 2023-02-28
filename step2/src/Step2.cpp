/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

#include <string>
#include <map>
#include <iwscapi.h>
#include <wscapi.h>
#include <iostream>

const std::wstring ANTIVIRUS_SOFTWARE = L"Antivirus software";
const std::wstring EXPIRED = L"Expired";
const std::wstring MICROSOFT = L"Microsoft";
const std::wstring OFF = L"Off";
const std::wstring ON = L"On";
const std::wstring UNDEFINED = L"Undefined";

/* This is a DTO to hold the basic description of an installed antivirus software. */
class AvSoftwareDescriptor
{
private:

public:
	std::wstring name;
	std::wstring description;
	std::wstring definitionUpdateTime;
	std::wstring definitionStatus;
	std::wstring version;
	std::wstring productState;

	AvSoftwareDescriptor(
		std::wstring name,
		std::wstring description,
		std::wstring definitionUpdateTime,
		std::wstring definitionStatus,
		std::wstring version,
		std::wstring productState) :
			name(name),
			description(description),
			definitionUpdateTime(definitionUpdateTime),
			definitionStatus(definitionStatus),
			version(version),
			productState(productState){}

	AvSoftwareDescriptor() {}

	/* Infer whether an antivirus software is thirdy part or not. The decision is based on the presence (or not)
	 * of "Microsoft" word in the product name.
	 *
	 * Input: none.
	 * Output: boolean.
	 * Exceptions: none.
	 */
	bool isThirdParty() {
		return name.find(MICROSOFT) == std::string::npos;
	}
};

/* This is a POJO to hold all installed antivirus software. */
class InstalledAvSoftwares {
private:
	/* A data container to hold all installed antivirus. */
	std::map<std::wstring, AvSoftwareDescriptor> installedAvSoftwaresMap;

public:
	/* Create a new antivirus descriptor and add it to the internal list.
	 *
	 * Input: the antivirus name, description, update time, status, version and state.
	 * Output: void.
	 * Exceptions: none.
	 */
	void addAvSoftware(
		std::wstring name,
		std::wstring description,
		std::wstring definitionUpdateTime,
		std::wstring definitionStatus,
		std::wstring version,
		std::wstring productState) {
		AvSoftwareDescriptor avSoftwareDescriptor = 
			AvSoftwareDescriptor(name, description, definitionUpdateTime, definitionStatus, version, productState);
		addAvSoftware(avSoftwareDescriptor);
	}

	/*
	 * Add new antivirus to the internal list.
	 *
	 * Input: the antivirus software descriptor.
	 * Output: void.
	 * Exceptions: none.
	 */
	void addAvSoftware(AvSoftwareDescriptor aVSoftwareDescriptor) {
		std::wstring name = aVSoftwareDescriptor.name;
		installedAvSoftwaresMap[name] = aVSoftwareDescriptor;
	}

	/*
	 * Return the antivirus internal list.
	 *
	 * Input: None.
	 * Output: The antivirus internal list.
	 * Exceptions: none.
	 */
	std::map<std::wstring, AvSoftwareDescriptor> getInstalledAvSoftwaresMap() {
		return installedAvSoftwaresMap;
	}
};

/* This is a class service to interact with WSC and provide some usefull methods to query products. */
class WscProductsService {
public:
	/* Below are some necessary objects to interact with WSC using COM. */
	HRESULT hr = S_OK;
	IWscProduct* PtrProduct = nullptr;
	IWSCProductList* PtrProductList = nullptr;
	BSTR PtrVal = nullptr;
	LONG ProductCount = 0;
	WSC_SECURITY_PRODUCT_STATE ProductState = WSC_SECURITY_PRODUCT_STATE_OFF;
	WSC_SECURITY_SIGNATURE_STATUS ProductStatus = WSC_SECURITY_PRODUCT_UP_TO_DATE;

	/* Below are the temporary objects used to adapt the WSC data to the present InstalledAvSoftwareDescriptor. */
	std::wstring displayName, state, timestamp, definitionState;

	/* Destructor will free used memory. */
	~WscProductsService() {
		SysFreeString(PtrVal);
		PtrProduct->Release();
	}

	/*
	 * Query Windows API for all installed antivirus software.
	 *
	 * Input: An InstalledAvSoftwares object to hold the results.
	 * Output: A boolean indicating if some antivirus were found.
	 * Exceptions: throw runtime_error if some of the Windows API tasks fail.
	 */
	bool queryWindowsForAVSoftwareDataWSC(InstalledAvSoftwares& installedAvSoftwares) {
		init();
		for (uint32_t i = 0; i < (int&)ProductCount; i++)
		{
			getItem(i);
			getProductName();
			getProductState();
			getProductSignatureStatus();
			getProductStateTimestamp();
			installedAvSoftwares.addAvSoftware(
				displayName,
				ANTIVIRUS_SOFTWARE,
				timestamp,
				definitionState,
				UNDEFINED,
				state);
		}
		
		return installedAvSoftwares.getInstalledAvSoftwaresMap().size() != 0;
	}

private:
	/* Initialize COM and get all installed antivirus software.  */
	void init() {
		coInitialize();
		coCreateInstance();
		initialize();
		getCount();
	}

	/*
	 * Call CoInitializeEx in the Windows API.
	 *
	 * Input: None.
	 * Output: None.
	 * Exceptions: throw runtime_error if some of the Windows API tasks fail.
	 */
	void coInitialize() {
		hr = CoInitializeEx(nullptr, 0);
		checkError("Failed to coInitializeEx the COM server. ");
	}

	/*
	 * Call coCreateInstance in the Windows API.
	 *
	 * Input: None.
	 * Output: None.
	 * Exceptions: throw runtime_error if some of the Windows API tasks fail.
	 */
	void coCreateInstance() {
		hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, 
				__uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
		checkError("Failed to create WSCProductList object. ");
	}

	/*
	 * Call WSC Initialize in the Windows API.
	 *
	 * Input: None.
	 * Output: None.
	 * Exceptions: throw runtime_error if some of the Windows API tasks fail.
	 */
	void initialize() {
		hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
		checkError("Failed to query antivirus product list. ");
	}

	/*
	 * Call get_Count in the Windows API.
	 *
	 * Input: None.
	 * Output: None.
	 * Exceptions: throw runtime_error if some of the Windows API tasks fail.
	 */
	void getCount() {
		hr = PtrProductList->get_Count(&ProductCount);
		checkError("Failed to query product count.");
	}

	/*
	 * Call get_Item in the Windows API.
	 *
	 * Input: None.
	 * Output: None.
	 * Exceptions: throw runtime_error if some of the Windows API tasks fail.
	 */
	void getItem(int i) {
		hr = PtrProductList->get_Item(i, &PtrProduct);
		checkError("Failed to query AV product.");
	}

	/*
	 * Call get_ProductName in the Windows API.
	 *
	 * Input: None.
	 * Output: None.
	 * Exceptions: throw runtime_error if some of the Windows API tasks fail.
	 */
	void getProductName() {
		hr = PtrProduct->get_ProductName(&PtrVal);
		displayName = std::wstring(PtrVal, SysStringLen(PtrVal));
		checkError("Failed to query AV product name.");
	}

	/*
	 * Call getProductState in the Windows API.
	 *
	 * Input: None.
	 * Output: None.
	 * Exceptions: throw runtime_error if some of the Windows API tasks fail.
	 */
	void getProductState() {
		hr = PtrProduct->get_ProductState(&ProductState);
		checkError("Failed to query AV product state.");
		if (ProductState == WSC_SECURITY_PRODUCT_STATE_ON)
		{
			state = ON;
		}
		else if (ProductState == WSC_SECURITY_PRODUCT_STATE_OFF)
		{
			state = OFF;
		}
		else
		{
			state = EXPIRED;
		}
	}

	/*
	 * Call getProductSignatureStatus in the Windows API.
	 *
	 * Input: None.
	 * Output: None.
	 * Exceptions: throw runtime_error if some of the Windows API tasks fail.
	 */
	void getProductSignatureStatus() {
		hr = PtrProduct->get_SignatureStatus(&ProductStatus);
		definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? L"UpToDate" : L"OutOfDate";
		checkError("Failed to query AV product signature status.");
	}

	/*
	 * Call getProductStateTimestamp in the Windows API.
	 *
	 * Input: None.
	 * Output: None.
	 * Exceptions: throw runtime_error if some of the Windows API tasks fail.
	 */
	void getProductStateTimestamp() {
		hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
		timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
		checkError("Failed to query AV product timestamp.");
	}

	/*
	 * Check whether the hr member of this class is indicating an error.
	 *
	 * Input: Desired error message.
	 * Output: None.
	 * Exceptions: throw runtime_error containing the specified error message.
	 */
	void checkError(std::string message) {
		if (FAILED(hr)) {
			throw std::runtime_error(message);
		}
	}
};

int main() {
	WscProductsService wscProductService;
	InstalledAvSoftwares installedAvSoftwares;

	try {
		wscProductService.queryWindowsForAVSoftwareDataWSC(installedAvSoftwares);
	}
	catch (const std::runtime_error ex) {
		std::cout << ex.what() << std::endl;
		return 0;
	}

	for (auto &av : installedAvSoftwares.getInstalledAvSoftwaresMap()) {
		if (av.second.isThirdParty()) {
			std::wcout << "Third party Antivirus found: " << av.second.name << std::endl;
		}
		else {
			std::wcout << "Microsoft Antivirus found: " << av.second.name << std::endl;
		}
	}
}