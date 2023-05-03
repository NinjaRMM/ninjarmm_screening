/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

// Add necessary headers for the used libraries and data types
#include <iostream>
#include <map>
#include <string>

#include <combaseapi.h>
#include <iwscapi.h>
#include <wscapi.h>
#include <winerror.h>

struct ThirdPartyAVSoftware
{
    // Please consider the following comments:
    // a. Consider using consistent style conventions (such as camelCase variable names. They are mixed on code)
    // b. Is there a specific reason for using std::wstring instead of std::string?
    // c. Ensure consistent data types throughout the structure (use std::wstring for string variables, for example)

    std::wstring Name;
    std::wstring Description; // On code, this is the same as ProductState, is that correct?
    std::wstring DefinitionUpdateTime;
    std::wstring DefinitionStatus; // Consider changing to std::wstring to match other string types used in the struct
    // std::wstring Version; // Unused variable, can be removed if not interested in obtaining the AV version
    std::wstring ProductState;

    // Consider adding a constructor to efficiently initialize instances of the struct with all necessary data
    ThirdPartyAVSoftware(const std::wstring &name = L"",
                         const std::wstring &description = L"",
                         const std::wstring &updateTime = L"",
                         const std::wstring &status = L"",
                         const std::wstring &state = L"")
        : Name(name),
          Description(description),
          DefinitionUpdateTime(updateTime),
          DefinitionStatus(status),
          ProductState(state)
    {
    }
};

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware> &thirdPartyAVSoftwareMap) // You can use an alias like o line 217
{
    // Please consider the followed comments:
    // 0. Part of my analysis was empirical, as I don't know much about COM objects
    // 1. Declare variables when needed to improve code readability and limit their scope. Also consider using style conventions (such variable names)
    // 2. Replace all instances of std::string with std::wstring to match the struct's member variables
    // 3. Although they aren't wrong, it would be interesting to replace the "cout" message error for "cerr", which indicates a better error message
    // 4. Depending on the context of the program, an exception may be more appropriate than simply returning false on failure (good to manage memory here)
    // 5. Group certain parts of the code into separate functions to improve modularity and readability (such as reading antivirus software inside the for loop)
    // 6. Review variable names that may not be optimal (e.g. PtrVal and PtrProduct)
    // 7. If applicable consider using smart pointers instead of raw pointers.

    // See comment 1
    HRESULT hr = S_OK;
    IWSCProductList *PtrProductList = nullptr;

    // std::wstring displayName, state, timestamp, definitionState; // See comment 1 and 2
    // Also remove the unused std::wstring versionNumber and change definitionState to a std::wstring to match the member variable

    // Use a linter to check line length and consider breaking up the line for readability
    hr = CoCreateInstance(__uuidof(WSCProductList), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID *>(&PtrProductList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. "; // See comment 3 and 4
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        PtrProductList->Release();                               // Missing product list release
        std::cout << "Failed to query antivirus product list. "; // See comment 3 and 4
        return false;
    }

    LONG ProductCount = 0; // See comment 1
    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        PtrProductList->Release();                      // Missing product list release
        std::cout << "Failed to query product count. "; // Consider adding an extra space to maintain the pattern of error messages in the code. Please also see comment 3 and 4
        return false;
    }

    for (auto i = 0; i < ProductCount; i++) // use auto to avoid mismatch here
    {
        IWscProduct *PtrProduct = nullptr; // See comment 1 and 6

        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product."; // See comment 3
            continue;
        }

        BSTR PtrName = nullptr; // Use a more descriptive variable name than "PtrVal". See comment 1 and 6.
        hr = PtrProduct->get_ProductName(&PtrName);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            std::cout << "Failed to query AV product name."; // See comment 3
            continue;
        }
        std::wstring displayName = std::wstring(PtrName, SysStringLen(PtrName)); // See comment 1
        SysFreeString(PtrName);                                                  // Caller is responsible for freeing the string

        WSC_SECURITY_PRODUCT_STATE ProductState; // See comment 1
        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            PtrProduct->Release();                            // Missing product release
            std::cout << "Failed to query AV product state."; // See comment 3. Consider adding "displayName" variable to the error message for better identification.
            continue;
        }

        // Consider refactoring the state variable assignment into a switch statement to improve code readability,
        // especially if additional states will be added in the future. For example:
        std::wstring state;
        switch (ProductState)
        {
        case WSC_SECURITY_PRODUCT_STATE_ON:
            state = L"On";
            break;
        case WSC_SECURITY_PRODUCT_STATE_OFF:
            state = L"Off";
            break;
        case WSC_SECURITY_PRODUCT_STATE_SNOOZED: // Missing state
            state = L"Snoozed";
            break;
        default:
            state = L"Expired";
            break;
        }

        WSC_SECURITY_SIGNATURE_STATUS ProductStatus; // See comment 1
        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            PtrProduct->Release();                                       // Missing product release
            std::cout << "Failed to query AV product definition state."; // See comment 3. Consider adding "displayName" variable to the error message for better identification.
            continue;
        }
        std::wstring definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? L"UpToDate" : L"OutOfDate";

        BSTR PtrTimestamp = nullptr;
        hr = PtrProduct->get_ProductStateTimestamp(&PtrTimestamp);
        if (FAILED(hr))
        {
            PtrProduct->Release();                                       // Missing product release
            std::cout << "Failed to query AV product definition state."; // See comment 3. Consider adding "displayName" variable to the error message for better identification.
            continue;
        }
        std::wstring timestamp = std::wstring(PtrTimestamp, SysStringLen(PtrTimestamp));
        SysFreeString(PtrTimestamp); // Caller is responsible for freeing the string

        // Use the ThirdPartyAVSoftware constructor to initialize new instances of the struct more efficiently
        ThirdPartyAVSoftware thirdPartyAVSoftware(displayName, state, timestamp, definitionState, state);
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        // As another option, the programmer could declare an instance of ThirdPartyAVSoftware at the beginning of the loop and fill it in accordingly
        // This way, declaring individual variables for each piece of data wouldn't be necessary (This is up to the developer's discretion)

        /*
        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = definitionState;
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = state; // This is the same as ProductState, is that correct?
        thirdPartyAVSoftware.ProductState = state;
        */

        PtrProduct->Release();
    }

    PtrProductList->Release(); // Missing list release

    // You can simplify the return and make it more readable
    /*
    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
    */
    return !thirdPartyAVSoftwareMap.empty(); // "return thirdPartyAVSoftwareMap.size();" is also valid
}

/**
 * This main was created to test the functionality of the queryWindowsForAVSoftwareDataWSC function.
 *
 * CMakeLists.txt:
 *     cmake_minimum_required(VERSION 3.1.0)
 *     project(step2)
 *     set(CMAKE_CXX_STANDARD 17)
 *     add_executable(CodeReview CodeReview.cpp)
 */
int main(int argc, char *argv[])
{
    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr))
    {
        std::cerr << "Failed to initialize COM library." << std::endl;
        return -1;
    }

    using ThirdPartyAVSoftwareMap = std::map<std::wstring, ThirdPartyAVSoftware>;
    ThirdPartyAVSoftwareMap thirdPartyAVSoftwareMap;

    if (const auto querySuccess = queryWindowsForAVSoftwareDataWSC(thirdPartyAVSoftwareMap); querySuccess)
    {
        std::cout << "Antivirus software information retrieved successfully!" << std::endl;

        for (const auto &[name, software] : thirdPartyAVSoftwareMap)
        {
            std::wcout << std::endl;
            std::wcout << L"Name: " << software.Name << std::endl;
            std::wcout << L"Description: " << software.Description << std::endl;
            std::wcout << L"Definition Update Time: " << software.DefinitionUpdateTime << std::endl;
            std::wcout << L"Definition Status: " << software.DefinitionStatus << std::endl;
            // std::wcout << L"Version: " << software.Version << std::endl;
            std::wcout << L"Product State: " << software.ProductState << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to retrieve Antivirus software information." << std::endl;
    }

    return 0;
}
