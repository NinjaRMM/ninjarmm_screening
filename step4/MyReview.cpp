// Joao Ferreira devcicerojr@gmail.com
// Code Review suggestions:

// My main suggestion would be to have only one 'return' statement
// at the end of the functions. In order to do that, I changed the
// original code of function queryWindowsForAVSoftwareDataWSC() and
// added an auxiliar function called 'failMsgToString()', that helps
// to print the proper error message according to the enum class object
// of type QuqeryErrorMessage.

// ADDITIONAL SUGGESTION:
// One more suggestion to the code would be to create wrapper classes to
// implement RAII (Resource Acquisition Is Initialization) on top of the Windows API raw pointers
// such as IWscProduct* PtrProduct, facilitating the resource management
//
// I am providing the "FINAL" version of the file after my code review (except for the RAII approach
// which I consider optional)
//------------------------------------------------

// ----- BEGINNING OF ADDED CODE
enum class QueryErrorMessage {
    FAIL_WSCPROD_OBJECT,
    FAIL_ANTIV_PROD_LIST,
    FAIL_PROD_COUNT,
    FAIL_QUERY_AV_PROD,
    FAIL_QUERY_AV_PROD_NAME,
    FAIL_QUERY_AV_PROD_STATE,
    FAIL_QUERY_AV_PROD_SIGN_STATUS,
    FAIL_QUERY_AV_PROD_DEF_STATE,
    FAIL_QUERY_AV_PROD_TIMESTAMP
}

// This auxiliar function allows us to print the proper message according to
// which failure happened
std::string failMsgToString(QueryErrorMessage fail_msg) {
    switch(fail_msg) {
        case FAIL_WSCPROD_OBJECT:
            ret_str = "Failed to create WSCProductList object.";
            break;
        case FAIL_ANTIV_PROD_LIST:
            ret_str = "Failed to query antivirus product list.";
            break;
        case FAIL_PROD_COUNT:
            ret_str = "Failed to query product count.";
            break;
        case FAIL_QUERY_AV_PROD:
            ret_str = "Failed to query AV product.";
            break;
        case FAIL_QUERY_AV_PROD_NAME:
            ret_str = "Failed to query AV product name.";
            break;
        case FAIL_QUERY_AV_PROD_STATE:
            ret_str = "Failed to query AV product state.";
            break;
        case FAIL_QUERY_AV_PROD_SIGN_STATUS:
            ret_str = "Failed to query AV product signature status.";
            break;
        case FAIL_QUERY_AV_PROD_DEF_STATE:
            ret_str = "Failed to query AV product definition state.";
            break;
        case FAIL_QUERY_AV_PROD_TIMESTAMP:
        default:
            ret_str = "Failed to query AV product state timestamp.";
            break;
    }
    return ret_str;
}
// ----- END OF ADDED CODE

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};


// ----- This function was refactored to have a single 'return' statement.
// Updating the introduced variable 'fail_msg' before every function that may 
// change the HRESULT 'hr' allows us to track what failed and print the error message.
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    bool retval = true;
    HRESULT hr = S_OK;
    IWscProduct* PtrProduct = nullptr;
    IWSCProductList* PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;

    // This variable is updated before we perform some action that may fail
    // so that we can track what failed in case hr returns a failure
    QueryErrorMessage fail_msg = FAIL_WSCPROD_OBJECT;
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (!FAILED(hr))
    {
        fail_msg = FAIL_ANTIV_PROD_LIST;
        hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    }

    if (!FAILED(hr))
    {
        fail_msg = FAIL_PROD_COUNT;
        hr = PtrProductList->get_Count(&ProductCount);
    }

    if (!FAILED(hr))
    {
        for (uint32_t i = 0; i < ProductCount; i++)
        {
            fail_msg = FAIL_QUERY_AV_PROD;
            hr = PtrProductList->get_Item(i, &PtrProduct);
            
            if (!FAILED(hr))
            {
                fail_msg = FAIL_QUERY_PROD_NAME;
                hr = PtrProduct->get_ProductName(&PtrVal);
            }

            if (!FAILED(hr))
            {
                displayName = std::wstring(PtrVal, SysStringLen(PtrVal));
                fail_msg = FAIL_QUERY_AV_PROD_STATE;
                hr = PtrProduct->get_ProductState(&ProductState);
            }

            if (!FAILED(hr))
            {
                switch (ProductState) {
                    case WSC_SECURITY_PRODUCT_STATE_ON:
                        state = L"On";
                        break;
                    case WSC_SECURITY_PRODUCT_STATE_OFF:
                        state = L"Off";
                        break;
                    default:
                        state = L"Expired";
                        break;
                }
                fail_msg = FAIL_QUERY_AV_PROD_SIGN_STATUS;
                hr = PtrProduct->get_SignatureStatus(&ProductStatus);                
            }

            if (!FAILED(hr))
            {
                definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";
                fail_msg = FAIL_QUERY_AV_PROD_TIMESTAMP;
                hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
            }          

            if (!FAILED(hr))
            {
                timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
                SysFreeString(PtrVal);
                
                ThirdPartyAVSoftware thirdPartyAVSoftware;
                thirdPartyAVSoftware.Name = displayName;
                thirdPartyAVSoftware.DefinitionStatus = definitionState;
                thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
                thirdPartyAVSoftware.Description = state;
                thirdPartyAVSoftware.ProductState = state;
                thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
            }
            // In case we get here with a failed HR, we can print what went wrong
            else {
                std::cout << failMsgToString(fail_msg);
            }
            
            if (PtrProduct != nullptr) {
                PtrProduct->Release();
            }
        }

        if (thirdPartyAVSoftwareMap.size() == 0)
        {
            retval = false;
        }

        return retval;
    }
}