/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

// This code does not compile, appropriate header files should be included
#include <string>
#include <iostream>
#include <combaseapi.h>
#include <iwscapi.h>
#include <wscapi.h>
#include <unordered_map>
#include <chrono>
#include <ctime>

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus; // There seems to be a typo and instead DefinitionStatus should be a wstring. Flagging in case this was not intended and a typo.
    std::wstring Version;
    std::wstring ProductState;
};

std::wstring ProductStateToString(WSC_SECURITY_PRODUCT_STATE val) {
  switch(val) {
    case WSC_SECURITY_PRODUCT_STATE_ON:
      return L"On";
    case WSC_SECURITY_PRODUCT_STATE_OFF:
      return L"Off";
    default:
      return L"Expired";
  }
}

std::string ProductStatusToString(WSC_SECURITY_SIGNATURE_STATUS val) {
  switch(val) {
    case WSC_SECURITY_PRODUCT_UP_TO_DATE:
      return "UpToDate";
    case WSC_SECURITY_PRODUCT_OUT_OF_DATE:
    default:
      return "OutOfDate";
  }
}

void logger(std::string_view str) {
  const auto now = std::chrono::system_clock::now();
  const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
  std::cout << "LOGINFO [" << t_c << "]: " << str << std::endl;
}

// bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap) // An std::unordered_map should be used instead of std::map for performance reasons since the use of the ordered functionality is not being used for the map.
bool queryWindowsForAVSoftwareDataWSC(std::unordered_map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    std::wstring displayName, versionNumber, timestamp;
    std::string definitionState;

    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        // std::cout << "Failed to create WSCProductList object. "; // A proper logger should be used with the timestamps and the error code received and preferably in a log file rather than the output stream
        const std::string error = "Failed to query antivirus product list.";
        logger(error);
        // return false; // It is better to throw an logic_error exception instead of return false that is a valid result to return. Here it's better to communicate than an error occurred in my opinion.
        throw std::logic_error(error);
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        PtrProductList->Release(); // We should release the PtrProductList memory, otherwise we have a leak
        const std::string error = "Failed to query antivirus product list.";
        // std::cout << "Failed to query antivirus product list. "; // A proper logger should be used with the timestamps and the error code received and preferably in a log file rather than the output stream
        logger(error);
        // return false; // It is better to throw an logic_error exception instead of return false that is a valid result to return. Here it's better to communicate than an error occurred in my opinion.
        throw std::logic_error(error);
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        PtrProductList->Release(); // We should release the PtrProductList memory, otherwise we have a leak
        const std::string error = "Failed to query product count.";
        //std::cout << "Failed to query product count."; // A proper logger should be used with the timestamps and the error code received and preferably in a log file rather than the output stream
        logger(error);
        // return false; // It is better to throw an logic_error exception instead of return false that is a valid result to return. Here it's better to communicate than an error occurred in my opinion.
        throw std::logic_error(error);
    }

    // for (uint32_t i = 0; i < ProductCount; i++) // Implicit conversions from LONG to uint32_t with possible loss of data, LONG should be used as the type of the iterator i
    for (LONG i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            // std::cout << "Failed to query AV product."; // A proper logger should be used with the timestamps and the error code received and preferably in a log file rather than the output stream
            logger("Failed to query AV product.");
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            // std::cout << "Failed to query AV product name."; // A proper logger should be used with the timestamps and the error code received and preferably in a log file rather than the output stream
            logger("Failed to query AV product name.");
            continue;
        }

        displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            PtrProduct->Release(); // We are leaking memory here since PtrProduct->Release(); is not called
            // std::cout << "Failed to query AV product state."; // A proper logger should be used with the timestamps and the error code received and preferably in a log file rather than the output stream
            logger("Failed to query AV product state.");
            continue;
        }

        // This code is not needed as it is bad practice to have hardcoded values in the code. It is replaced by the creation of a ToString fuction.
        // if (ProductState == WSC_SECURITY_PRODUCT_STATE_ON)
        // {
        //     state = L"On";
        // }
        // else if (ProductState == WSC_SECURITY_PRODUCT_STATE_OFF)
        // {
        //     state = L"Off";
        // }
        // else
        // {
        //     state = L"Expired";
        // }

        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            PtrProduct->Release(); // We are leaking memory here since PtrProduct->Release(); is not called
            // std::cout << "Failed to query AV product definition state."; // A proper logger should be used with the timestamps and the error code received and preferably in a log file rather than the output stream
            logger("Failed to query AV product definition state.");
            continue;
        }

        // This code is not needed as it is bad practice to have hardcoded values in the code. It is replaced by the creation of a ToString fuction
        // definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release(); // We are leaking memory here since PtrProduct->Release(); is not called
            // std::cout << "Failed to query AV product definition state."; // A proper logger should be used with the timestamps and the error code received and preferably in a log file rather than the output stream
            logger("Failed to query AV product definition state.");
            continue;
        }
        timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);
        PtrVal = nullptr; // We should assign PtrVal back to a nullptr value after this call for best practice, it will avoid dealing with corrupted memory

        ThirdPartyAVSoftware thirdPartyAVSoftware;
        thirdPartyAVSoftware.Name = displayName;
        thirdPartyAVSoftware.DefinitionStatus = ProductStatusToString(ProductStatus);
        thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
        thirdPartyAVSoftware.Description = ProductStateToString(ProductState);
        thirdPartyAVSoftware.ProductState = ProductStateToString(ProductState);
        thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

        PtrProduct->Release();
        PtrProduct = nullptr; // We should assign PtrProduct back to a nullptr value after this call for best practice, it will avoid dealing with corrupted memory
    }

    PtrProductList->Release(); // Memory leak, we should call release to free the memory
    PtrProductList = nullptr; // We should assign PtrProductList back to a nullptr value after this call for best practice, it will avoid dealing with corrupted memory

    if (thirdPartyAVSoftwareMap.size() == 0)
    {
        return false;
    }
    return true;
}