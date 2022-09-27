#include "WMIService.h"


WMIService::WMIService() : pSvc(nullptr), pLoc(nullptr)
{
	Initialize();
	
}
WMIService::~WMIService()
{
	Release();
}

void WMIService::Initialize()
{
	if (pLoc && pSvc)
		return;
	HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres)) {
		throw std::exception("Failed to initialize COM library");
	}

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------

	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
	);


	if (FAILED(hres)) {
		CoUninitialize();
		throw std::exception("Failed to initialize COM security levels");
	}

	//obtain the initial locator to WMI
	pLoc = nullptr;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID*)&pLoc);

	if (FAILED(hres)) {
		CoUninitialize();
		throw std::exception("Failed to create IWbemLocator object");
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method
	pSvc = nullptr;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (for example, Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
	);

	if (FAILED(hres)) {
		pLoc->Release();
		CoUninitialize();
		throw std::exception("Could not connect to root/cimv2 namespace");
	}


	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
	);

	if (FAILED(hres)) {
		Release();
		throw std::exception("Could not set proxy blanket");
	}
}

void WMIService::Release()
{
	if (pSvc && pLoc)
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
	}
}

bstr_t WMIService::BuildQueryString(const wchar_t* wmiClass, std::vector<LPCWSTR> attrs) 
{
	// we suppose 25 is always enough. If not use, then we would use wstring class and recover buffer
	WCHAR queryString[256] = { 0 };
	wcscpy(queryString, L"SELECT ");
	auto it = attrs.begin();
	for (auto it = attrs.begin(); it != attrs.end(); it++) {
		wcscat(queryString, *it);
		if (!((attrs.end() - it == 1))) {
			wcscat(queryString, L", ");
		}
	}
	wcscat(queryString, L" FROM ");
	wcscat(queryString, wmiClass);
	return _bstr_t(queryString);
}
IEnumWbemClassObject* WMIService::ExecuteWQLQuery(const wchar_t* wmiClass, const std::vector<LPCWSTR> &attrs)
{
	_bstr_t query = BuildQueryString(wmiClass, attrs);
	return ExecuteWQLQuery(query);
}
IEnumWbemClassObject* WMIService::ExecuteWQLQuery(BSTR stringQuery)
{
	IEnumWbemClassObject* pEnumerator = nullptr;
	HRESULT hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		stringQuery,
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres)) {
		throw std::exception("Fatal error: Query to operating system failed");
		Release();
	}
	else {
		return pEnumerator;
	}
}


WMIWBEMINFO* WMIWBEMINFO::currentInstance;

WMIWBEMINFO* WMIWBEMINFO::getWMIWBEMINFOInstance() {
	if (currentInstance == nullptr) {
		currentInstance = new WMIWBEMINFO();
	}
	return currentInstance;
}

IWbemLocator* WMIWBEMINFO::getWbemLocator() {
	return this->pLoc;
}

IWbemServices* WMIWBEMINFO::getWbemServices() {
	return this->pSvc;
}

HRESULT WMIWBEMINFO::getHres() {
	return this->hres;
}

WMIWBEMINFO::~WMIWBEMINFO() {
	this->pSvc->Release();
	this->pLoc->Release();
	CoUninitialize();
}
