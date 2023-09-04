#pragma once

#include <iostream>

/* 
   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   <<<<<<<<<<< BEGIN - JUST CODE TO AVOID ERRORS IN STATIC ANALYSIS. PLEASE IGNORE IT <<<<<<
   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

typedef enum _WSC_SECURITY_PROVIDER
{
    WSC_SECURITY_PROVIDER_FIREWALL = 0x1,
    WSC_SECURITY_PROVIDER_AUTOUPDATE_SETTINGS = 0x2,
    WSC_SECURITY_PROVIDER_ANTIVIRUS = 0x4,
    WSC_SECURITY_PROVIDER_ANTISPYWARE = 0x8,
    WSC_SECURITY_PROVIDER_INTERNET_SETTINGS = 0x10,
    WSC_SECURITY_PROVIDER_USER_ACCOUNT_CONTROL = 0x20,
    WSC_SECURITY_PROVIDER_SERVICE = 0x40,
    WSC_SECURITY_PROVIDER_NONE = 0,
    WSC_SECURITY_PROVIDER_ALL
} WSC_SECURITY_PROVIDER, *PWSC_SECURITY_PROVIDER;

typedef enum WSC_SECURITY_PRODUCT_STATE
{
    WSC_SECURITY_PRODUCT_STATE_ON = 0,
    WSC_SECURITY_PRODUCT_STATE_OFF = 1,
    WSC_SECURITY_PRODUCT_STATE_SNOOZED = 2,
    WSC_SECURITY_PRODUCT_STATE_EXPIRED = 3
};

typedef enum _WSC_SECURITY_SIGNATURE_STATUS
{
    WSC_SECURITY_PRODUCT_OUT_OF_DATE = 0,
    WSC_SECURITY_PRODUCT_UP_TO_DATE = 1
} WSC_SECURITY_SIGNATURE_STATUS;

typedef enum HRESULT
{
    S_OK = 0,
    S_NOK = 1
} HRESULT;

typedef char* BSTR;
typedef long LONG;

void SysFreeString(BSTR b) { std::cout << "SysFreeString" << std::endl; }
size_t SysStringLen(BSTR b) { std::cout << "SysStringLen" << std::endl; }

class IWscProduct
{
public:
    virtual HRESULT get_ProductName(BSTR *pVal) = 0;
    virtual HRESULT get_ProductState(WSC_SECURITY_PRODUCT_STATE *pVal) = 0;
    virtual HRESULT get_ProductStateTimestamp(BSTR *pVal) = 0;
    virtual HRESULT get_SignatureStatus(WSC_SECURITY_SIGNATURE_STATUS *pVal) = 0;
    void Release() { std::cout << "release" << std::endl; }
};

bool FAILED(HRESULT hr) { return hr != S_OK; }

class IWSCProductList
{
public:
    HRESULT Initialize(WSC_SECURITY_PROVIDER p) { return S_OK; }
    HRESULT get_Count(LONG* c) { *c = 5L; return S_OK; }
    HRESULT get_Item(uint32_t i, IWscProduct** prod) { return S_OK; }
};

class WSCProductList
{
};

/* 
   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   <<<<<<<<<<< END - JUST CODE TO AVOID ERRORS IN STATIC ANALYSIS. PLEASE IGNORE IT <<<<<<
   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/
