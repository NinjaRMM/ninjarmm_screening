#pragma once

#include "Win32APIEnumsAnsTypes.h"
#include <string>


// Maps Win32 API's ProductState values to our description pattern
class ProductStateMapper
{
public:
    inline static const std::wstring ON = L"On";
    inline static const std::wstring OFF = L"Off";
    inline static const std::wstring EXPIRED = L"Expired";
    
    static std::wstring Get(WSC_SECURITY_PRODUCT_STATE state)
    {
        std::wstring description;

        switch (state)
        {
            case WSC_SECURITY_PRODUCT_STATE_ON:
                description = ON;
                break;

            case WSC_SECURITY_PRODUCT_STATE_OFF:
                description = OFF;
                break;

            case WSC_SECURITY_PRODUCT_STATE_SNOOZED:
                [[fallthrough]];

            case WSC_SECURITY_PRODUCT_STATE_EXPIRED:
                [[fallthrough]];  

            default:
                description = EXPIRED;
                break;
        }

        return description;
    }
};

// Maps Win32 API's SignatureStatus values to our description pattern
class DefinitionStateMapper
{
public:
    inline static const std::string UP_TO_DATE = "UpToDate";
    inline static const std::string OUT_OF_DATE = "OutOfDate";

    static std::string Get(WSC_SECURITY_SIGNATURE_STATUS status)
    {
        if (status == WSC_SECURITY_PRODUCT_UP_TO_DATE)
        {
            return UP_TO_DATE;
        }

        return OUT_OF_DATE;
    }
};