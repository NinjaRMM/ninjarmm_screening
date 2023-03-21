/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

/************** OVERALL COMMENTS ******************************************
 * No headers necessary for compilation of the file
 * Inconsistent variable types
 * Inconsistent variable naming
 * I would recommend against using variable defines for types and for values with the same structure: i.e: WSC_SECURITY_PRODUCT_STATE, WSC_SECURITY_PRODUCT_STATE_ON
 * Also the coding seems to mix a lot of snake case and camel case, however I am unsure of company policy
 * Capitalized / uncapitalized letters.
 * No unit testing
 *****************************************************************************/

// Lack of standard C++ headers
#include <string>
#include <map>
#include <iostream>
#include <cstring>
#include <vector>

// These are for code adaptations to run in linux and should be disregarded
#include <locale>
#include <codecvt>

// Missing Windows includes, since I did this on linux I'm not sure if I missed any
// #include <windows.h>
// #include <objbase.h>
// #include <OleAuto.h>
// #include <WinError.h>
// #include <WinDef.h>

#define WSC_SECURITY_PRODUCT_STATE std::wstring
#define WSC_SECURITY_SIGNATURE_STATUS std::wstring

// Testing defines for compilation, should be discarded
#define WSC_SECURITY_PRODUCT_STATE_ON L"On"
#define WSC_SECURITY_PRODUCT_STATE_OFF L"Off"
#define WSC_SECURITY_PRODUCT_UP_TO_DATE L"1.0.0"
#define WSC_SECURITY_PROVIDER_ANTIVIRUS L"NinjaAV"

// Testing defines for Linux, should be discarded
#define HRESULT int16_t
#define LONG unsigned int
#define S_OK 0
#define FAILED(a) (a != 0)
#define BSTR char *

struct ThirdPartyAVSoftware
{
    // This is a windows application (due to the use of CoCreateInstance, __uuidof, LPVOID etc), so most likely everything
    // should use wstring in order to accomodate various characters.
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::wstring DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

// Mock class created for compiling this code, should be disregarded with the correct header included
class IWscProduct
{
private:
    std::string name;
    std::wstring state;
    std::wstring signatureStatus;
    std::string stateTimestamp;

public:
    HRESULT get_ProductName(BSTR *out)
    {
        size_t len = name.length();
        char *buffer = new char[len + 1];
        strcpy(buffer, name.c_str());
        *out = buffer;
        return S_OK;
    }
    HRESULT get_ProductState(std::wstring *out)
    {
        *out = state;
        return S_OK;
    }
    HRESULT get_SignatureStatus(std::wstring *out)
    {
        *out = signatureStatus;
        return S_OK;
    }
    HRESULT get_ProductStateTimestamp(BSTR *out)
    {
        char *charTS = const_cast<char *>(stateTimestamp.c_str());
        *out = charTS;
        return S_OK;
    }
    // Using the Release function to print object information. it should be used to release memory
    HRESULT Release()
    {
        std::cout << "--- Release Testing Data ---" << std::endl;
        std::cout << name << " " << stateTimestamp << "  ";
        std::wcout << state << " " << signatureStatus << std::endl;
        std::cout << "--- Release Testing Data ---" << std::endl;
        return S_OK;
    }

    void set_name(std::string newName)
    {
        name = newName;
    }
    void set_state(std::wstring newState)
    {
        state = newState;
    }

    void set_signatureStatus(std::wstring newSigStatus)
    {
        signatureStatus = newSigStatus;
    }

    void setStateTimestamp(std::string newStateTimestamp)
    {
        stateTimestamp = newStateTimestamp;
    }

    ~IWscProduct() {}
    IWscProduct() {}
};

class IWSCProductList
{
private:
    std::vector<IWscProduct *> theList;
    LONG count = 0;

public:
    // In the real code this function probably retrieves information from a database, so I'll just be mocking a small list.
    HRESULT Initialize(std::wstring)
    {
        IWscProduct *aux;
        aux = new IWscProduct();
        aux->set_name("AV 1");
        aux->set_state(L"On");
        aux->set_signatureStatus(L"1.0.0");
        aux->setStateTimestamp("100000000");
        theList.push_back(aux);
        aux = new IWscProduct();
        aux->set_name("AV 2");
        aux->set_state(L"Off");
        aux->set_signatureStatus(L"0.0.9");
        aux->setStateTimestamp("1000000");
        theList.push_back(aux);
        aux = new IWscProduct();
        aux->set_name("AV 3");
        aux->set_state(L"Invalid");
        aux->set_signatureStatus(L"XXX");
        aux->setStateTimestamp("100");
        theList.push_back(aux);
        aux = nullptr;
        count = 3;
        return S_OK;
    }
    HRESULT get_Count(LONG *ct)
    {
        *ct = count;
        return S_OK;
    }
    HRESULT get_Item(uint32_t pos, IWscProduct **prod)
    {
        *prod = theList.at(pos);
        return S_OK;
    }
};

// Mock functions for compiling
size_t SysStringLen(BSTR str)
{
    return strlen(str);
}

void SysFreeString(BSTR str)
{
    // This is not ideal as we should free the memory allocated for the strings
    return;
}

std::wstring StringToWideString(BSTR str)
{
    std::string ptrValRegString(str);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(ptrValRegString);
}

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware> &thirdPartyAVSoftwareMap)
{
    // Lots of types here not accounted for, probably from a missing header inclusion or missing definitions for this file.
    // We can try and infer a couple of them by context.
    HRESULT hr = S_OK;

    // IWscProduct and IWSCProductList should have a consistent beginning of wording.
    // By the use in the CoCreateInstance, this is an object and the header wasn't included
    IWscProduct *PtrProduct = nullptr;

    // By the use in the CoCreateInstance, this is an object and the header wasn't included
    IWSCProductList *PtrProductList = nullptr;

    BSTR PtrVal = nullptr;

    LONG ProductCount = 0;

    // Product state in the structure is a wide string, so we will assume it applies here as well
    WSC_SECURITY_PRODUCT_STATE ProductState;

    // The same will be done for Signature status, which might be refering to DefinitionStatus
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    std::wstring displayName, versionNumber, state, timestamp;
    // Again not defined as a wstring
    // Maybe this was done due to BSTR conversion
    std::wstring definitionState;

    // Windows specific function, since I'm using linux for this assignment, I will just set hr to OK to proceed with this part of the code.
    // Also for linux testing, we will initialize the mock PtrProductList object
    // This two lines below should be deleted and the other one uncommented for final commit
    PtrProductList = new IWSCProductList();
    hr = S_OK;
    // hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));

    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }
    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        return false;
    }

    for (uint32_t i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            std::cout << "Failed to query AV product name.";
            continue;
        }
        // The wide string creation with SysStringLen wll work on windows but not linux, so I replaced it in order to compile
        // Delete the line below and uncomment the regular one for Windows
        displayName = StringToWideString(PtrVal);
        // displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product state.";
            continue;
        }

        if (ProductState == WSC_SECURITY_PRODUCT_STATE_ON)
        {
            state = L"On";
        }
        else if (ProductState == WSC_SECURITY_PRODUCT_STATE_OFF)
        {
            state = L"Off";
        }
        else
        {
            state = L"Expired";
        }

        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        // Added L to specify wide string
        definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? L"UpToDate" : L"OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            continue;
        }

        // The wide string creation with SysStringLen wll work on windows but not linux, so I replaced it in order to compile
        // Delete the line below and uncomment the regular one for Windows
        timestamp = StringToWideString(PtrVal);
        // timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));

        SysFreeString(PtrVal);

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
    }

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}

// Mock main function to test functionalities
int main()
{
    std::map<std::wstring, ThirdPartyAVSoftware> testMap;
    bool res = queryWindowsForAVSoftwareDataWSC(testMap);

    std::cout << "Final result: " << res << std::endl;
    std::cout << "--- Printing Final Map Data ---" << std::endl;
    for (const auto &elem : testMap)
    {
        std::wcout << elem.second.Name << " " << elem.second.Description << std::endl;
        std::wcout << elem.second.DefinitionStatus << " " << elem.second.ProductState << " " << elem.second.Version << std::endl;
    }

    return 0;
}