/*  Developed and tested on :
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
 * This file contains the 'main' function. Program execution begins and ends there.
*/

#include <iostream>
#include <windows.h>
#include <iwscapi.h>
#include <wscapi.h>
#include <map>
#include <string>

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

//--- Function bool queryWindowsForAVSoftwareDataWSCImp(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
//--- Pulls all the data out and stores it in the Map Structure
bool queryWindowsForAVSoftwareDataWSCImp(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;
    IWscProduct* ptrProduct = nullptr;
    IWSCProductList* ptrProductList = nullptr;
    BSTR ptrVal = nullptr;
    LONG productCount = 0;
    WSC_SECURITY_PRODUCT_STATE productState;
    WSC_SECURITY_SIGNATURE_STATUS productStatus;

    std::wstring displayName, state, timestamp;
    std::string definitionState;

    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&ptrProductList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    //  ptrProductList Valid

    hr = ptrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        ptrProductList->Release();
        return false;
    }

    hr = ptrProductList->get_Count(&productCount);
    if (FAILED(hr))
    {
        std::cout << "Failed to query product count.";
        ptrProductList->Release();
        return false;
    }

    std::cout << "Antivirus products installed detected: " << productCount << "\n";

    for (LONG i = 0; i < productCount; i++)
    {
        displayName = L"<unable to retrive value for AV product #" + std::to_wstring(i + 1) + L">";
        definitionState = "<unable to retrive value>";
        timestamp = L"<unable to retrive value>";
        state = L"<unable to retrive value>";

        hr = ptrProductList->get_Item(i, &ptrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
        }
        else //  ptrProduct Valid
        {
            hr = ptrProduct->get_ProductName(&ptrVal);
            if (FAILED(hr))
            {
                std::cout << "Failed to query AV product name.";
            }
            else
            {
                displayName = std::wstring(ptrVal, SysStringLen(ptrVal));
            }

            hr = ptrProduct->get_ProductState(&productState);
            if (FAILED(hr))
            {
                std::cout << "Failed to query AV product state.";
            }
            else
            {
                if (productState == WSC_SECURITY_PRODUCT_STATE_ON)
                {
                    state = L"On";
                }
                else if (productState == WSC_SECURITY_PRODUCT_STATE_OFF)
                {
                    state = L"Off";
                }
                else
                {
                    state = L"Expired";
                }
            }

            hr = ptrProduct->get_SignatureStatus(&productStatus);
            if (FAILED(hr))
            {
                std::cout << "Failed to query AV product definition state.";
            }
            else
            {
                definitionState = (productStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";
            }

            hr = ptrProduct->get_ProductStateTimestamp(&ptrVal);
            if (FAILED(hr))
            {
                std::cout << "Failed to query AV product definition state.";
            }
            else
            {
                timestamp = std::wstring(ptrVal, SysStringLen(ptrVal));
                SysFreeString(ptrVal);
            }
        }

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state;
        thirdPartyAVSoftware.ProductState = state;
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        if (ptrProduct != nullptr)
        {
            ptrProduct->Release();
        }
    }

    ptrProductList->Release();

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}

//--- Function bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
//--- Original function, now it calls queryWindowsForAVSoftwareDataWSCImp that pulls the data
//--- COM Initialization is handles here
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hrCoInitialize = S_FALSE;

    hrCoInitialize = CoInitialize(NULL);
    if (hrCoInitialize != S_OK)
    {
        if (hrCoInitialize != S_FALSE)
        {
            std::cout << "COM library already Initialized on the current thread, continue";
            // we can continue
        }
        std::cout << "Failed initializing the COM library on the current thread";
        return false;
    }

    bool bReturnValue = queryWindowsForAVSoftwareDataWSCImp(thirdPartyAVSoftwareMap);

    // calling UnCoInitialize only if it was called here
    if (hrCoInitialize == S_OK)
    {
        CoUninitialize();
    }

    return bReturnValue;
}

int main()
{
    std::map<std::wstring, ThirdPartyAVSoftware> Map;
    int counter = 1;

    bool bResult = queryWindowsForAVSoftwareDataWSC(Map);

    std::cout << "\n\nRetrived Data for: " << Map.size() << " AV Porducts\n";

    for (std::map<std::wstring, ThirdPartyAVSoftware>::iterator itr = Map.begin(); itr != Map.end(); counter++, ++itr)
    {
        std::wcout << "\n"
            << "Product #" << counter << "\n"
            << "\tDisplay Name: " << itr->second.Name << "\n"
            << "\tProduct State: " << itr->second.ProductState << "\n"
            << "\tDefinition Update Time: " << itr->second.DefinitionUpdateTime
            << "\n\n";
    }

    std::cout << '\n' << "Press ENTER to finish...";
    std::cin.get();

}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
