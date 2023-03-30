/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/
//I clang-formatted the file

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string  DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

/**
 * @brief       Created enumerator to better handle different return values
 * @warning     This might break code that calls it. Need to check all instances
 */
typedef enum QueryWinErr_e
{
    AV_SW_RETURN_OK      = 0,  ///< Return OK
    AV_SW_CREAT_ERR      = -1, ///< Creation error
    AV_SW_INIT_ERR       = -2, ///< Initialization error
    AV_SW_NONE_AVAIL_ERR = -3, ///< No Third Party AV SW available error
} QueryWinErr_et;

QueryWinErr_et queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap, HRESULT errcode* = nullptr)
{
    QueryWinErr_et ret = AV_SW_CREAT_ERR; // Created return variable to remove multiple points of return.

    HRESULT hr = S_OK; //maintained the scope of this variable to optionally return as a parameter
    //All variables were moved to the smallest scope possible or removed if not necessary

    // Some standards tend to not allow "break" or "continue" but I use them to reduce overall cyclomatic complexity, depth and improve
    // readability

    // The following "do while(0)" is a trick I'd like to do to avoid multiple points of return without increasing
    // code depth which usually comes when combining "if" statements
    do
    {
        IWSCProductList* PtrProductList = nullptr; // reduced the scope of the variable
        LONG             ProductCount   = 0;       // reduced the scope of the variable

        hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
        if (FAILED(hr))
        {
            //no need to set the first error, since it's the default value.
            std::cout << "Failed to create WSCProductList object. ";
            break;
        }

        hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
        if (FAILED(hr))
        {
            std::cout << "Failed to query antivirus product list. ";
            ret = AV_SW_INIT_ERR;
            break;
        }
        hr = PtrProductList->get_Count(&ProductCount);
        if (FAILED(hr))
        {
            std::cout << "Failed to query product count.";
            ret = AV_SW_GET_COUNT_ERR;
            break;
        }
        for (uint32_t i = 0; i < ProductCount; ++i) // doesn't make a difference in functionality but might lead to more safe/optimized assembly
        {

            IWscProduct* PtrProduct = nullptr; //reduced the scope of the variable

            hr = PtrProductList->get_Item(i, &PtrProduct);
            if (FAILED(hr))
            {
                std::cout << "Failed to query AV product at Product " << i; //if it fails better to know which index
                continue;
            }

            BSTR PtrVal = nullptr; //reduced the scope of this variable

            hr = PtrProduct->get_ProductName(&PtrVal);
            if (FAILED(hr))
            {
                PtrProduct->Release();
                std::cout << "Failed to query AV product name at Product " << i; //if it fails better to know which index
                continue;
            }

            std::wstring displayName; // reduced scope of the variable

            displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

            WSC_SECURITY_PRODUCT_STATE CurrProductState = WSC_SECURITY_PRODUCT_STATE_ON; // Renamed to avoid confusion and shortened scope

            hr = PtrProduct->get_ProductState(&CurrProductState);
            if (FAILED(hr))
            {
                std::cout << "Failed to query AV product state at Product " << i; //if it fails better to know which index
                continue;
            }

            std::wstring curr_state = L"Expired"; // Shortened scope and default value to reduce else

            if (CurrProductState == WSC_SECURITY_PRODUCT_STATE_ON)
            {
                curr_state = L"On";
            }
            else if (CurrProductState == WSC_SECURITY_PRODUCT_STATE_OFF)
            {
                curr_state = L"Off";
            }

            WSC_SECURITY_SIGNATURE_STATUS CurrProductStatus = WSC_SECURITY_PRODUCT_OUT_OF_DATE; // Renamed and shortened scope

            hr = PtrProduct->get_SignatureStatus(&CurrProductStatus);
            if (FAILED(hr))
            {
                std::cout << "Failed to query AV product definition state at Product " << i; //if it fails better to know which index
                continue;
            }

            hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
            if (FAILED(hr))
            {
                std::cout << "Failed to query AV product definition state at Product " << i; //if it fails better to know which index
                continue;
            }

            //removed unecessary temporary struct.
            thirdPartyAVSoftwareMap[displayName].Name                 = displayName;
            thirdPartyAVSoftwareMap[displayName].DefinitionStatus     = (CurrProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";
            thirdPartyAVSoftwareMap[displayName].DefinitionUpdateTime = std::wstring(PtrVal, SysStringLen(PtrVal));
            thirdPartyAVSoftwareMap[displayName].Description          = curr_state;
            thirdPartyAVSoftwareMap[displayName].ProductState         = curr_state;

            SysFreeString(PtrVal);
            PtrProduct->Release();
        }
        if (thirdPartyAVSoftwareMap.size() == 0)
        {
            ret = AV_SW_NONE_AVAIL_ERR;
            break;
        }

        ret = AV_SW_RETURN_OK;
    } while (0);

    if (errcode)
    {
        *errcode = hr; //this will return the code even if the function results in success to some extent.
        //If both return value and errcode are ok, than it means it didn't fail in any product as well.
    }

    return ret;
}