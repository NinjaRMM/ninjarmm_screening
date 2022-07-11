/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

//# Since this is not an actual CodeReview and I cannot make sidecomments like in GitHub, Im marking
//# those with //#, so separate from normal comments

#include <iostream>
#include <iwscapi.h>
#include <map>
#include <optional>
#include <string>
#include <windows.h>
#include <wscapi.h>

//# I have changed these two a enum because its defined by us, and doesn't make sense to have free
//# values, eventually someone will have a typo and it will be a mess.
enum class ProductState
{
    On,
    Off,
    Expired
};

enum class DefinitionStatus
{
    //# This could be a bool right now, "isUpToDate", but this enum is more flexible and we could
    //# have more values in the future
    UpToDate,
    OutOfDate
};

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::wstring Version;

    //# The reordering is not something I would normally do in a commit, it can impact serialization
    //# and potentially packing. But if the code was _new_ I would put these two below, because unlike
    //# the rest of the code, these are defined by our app.
    //# Even without the enums, I wouldnt have a std::string mixed with all the std::wstring. Either
    //# work all with std::string utf8, and do translations to UTF16 before Windows calls, or keep UTF16

    ProductState State;
    DefinitionStatus Definition;
};

//# The main content of the initial function has been moved to a new one for cleanliness and easier testing
std::optional<ThirdPartyAVSoftware> parseThirdPartyAVSoftware(IWSCProductList *ptrProductList, LONG index)
{
    IWscProduct *ptrProduct = nullptr;
    HRESULT result = ptrProductList->get_Item(index, &ptrProduct);
    if (FAILED(result))
    {
        std::cout << "Failed to query AV product " << index << ". Error: " << result;
        return std::nullopt;
    }

    BSTR ptrVal = nullptr;
    result = ptrProduct->get_ProductName(&ptrVal);
    if (FAILED(result))
    {
        ptrProduct->Release();
        std::cout << "Failed to query AV product " << index << " name. Error: " << result;
        return std::nullopt;
    }

    ThirdPartyAVSoftware thirdPartyAVSoftware;
    thirdPartyAVSoftware.Name = std::wstring(ptrVal, SysStringLen(ptrVal));
    SysFreeString(ptrVal); //# Don't forget to free after usage!
    ptrVal = nullptr;

    WSC_SECURITY_PRODUCT_STATE productState;
    result = ptrProduct->get_ProductState(&productState);
    if (FAILED(result))
    {
        ptrProduct->Release(); //# Forgot the release!
        ptrProduct = nullptr;
        std::cout << "Failed to query AV product " << index << " state. Error: " << result;
        return std::nullopt;
    }

    if (productState == WSC_SECURITY_PRODUCT_STATE_ON)
    {
        thirdPartyAVSoftware.State = ProductState::On;
    }
    else if (productState == WSC_SECURITY_PRODUCT_STATE_OFF)
    {
        thirdPartyAVSoftware.State = ProductState::Off;
    }
    else
    {
        //# This will set Snoozed to Expired... Which may be inteded, but its weird
        thirdPartyAVSoftware.State = ProductState::Expired;
    }

    WSC_SECURITY_SIGNATURE_STATUS productStatus;
    result = ptrProduct->get_SignatureStatus(&productStatus);
    if (FAILED(result))
    {
        ptrProduct->Release(); //# Forgot the release!
        ptrProduct = nullptr;
        std::cout << "Failed to query AV product " << index << " definition state. Error: " << result;
        return std::nullopt;
    }

    thirdPartyAVSoftware.Definition =
        (productStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? DefinitionStatus::UpToDate : DefinitionStatus::OutOfDate;

    result = ptrProduct->get_ProductStateTimestamp(&ptrVal);
    if (FAILED(result))
    {
        ptrProduct->Release(); //# Forgot the release!
        ptrProduct = nullptr;
        std::cout << "Failed to query AV product " << index << " definition state. Error: " << result;
        return std::nullopt;
    }
    thirdPartyAVSoftware.DefinitionUpdateTime = std::wstring(ptrVal, SysStringLen(ptrVal));
    SysFreeString(ptrVal);
    ptrVal = nullptr;

    ptrProduct->Release();
    ptrProduct = nullptr;

    return thirdPartyAVSoftware;
}

//# Normally I would say to use unordered_map instead of map, but this time the number of elements will be
//# so low that it doesn't matter and order may be preferable.
//#
//# The return has also been changed. Instead of having a reference provided by the called, which can be
//# dangerous because it can already have values (which I doubt is intended, but could be), I think
//# is better to return it. Since it may not be succesful, I recommend wrapping it in std::optional.
//# Using std::expected with an appropiate errortype can be better, but that depends on the rest of the
//# codebase.
std::optional<std::map<std::wstring, ThirdPartyAVSoftware>> queryWindowsForAVSoftwareDataWSC()
{
    //# Unless this is compiling on very old C++ version we don't need to declare all variables on the
    //# function start. Also many dont need to be variables at all!

    //# Also CoInitialize/CoUninitialize are missing, but I guess that's just because this code is
    //# yoinked from some other part where its already called

    IWSCProductList *ptrProductList = nullptr;
    HRESULT result = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList),
                                      reinterpret_cast<LPVOID *>(&ptrProductList));
    if (FAILED(result))
    {
        //# In general I would avoid having stdoutput inside functionality. It's better to have some kind of logging
        //# function that can be easily accesed, and if appropiate, have it print the output, or write to file,
        //# or whatever is wanted. 
        std::cout << "Failed to create WSCProductList object. Error: " << result;
        return std::nullopt;
    }

    result = ptrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(result))
    {
        std::cout << "Failed to query antivirus product list. Error: " << result;
        return std::nullopt;
    }

    LONG ProductCount = 0;
    result = ptrProductList->get_Count(&ProductCount);
    if (FAILED(result))
    {
        std::cout << "Failed to query product count. Error: " << result;
        return std::nullopt;
    }

    //# The new name reflects the key used
    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareByName;

    //# uint32_t will be same as LONG on windows, but its still better practice to always use the same size
    //# as the iterated
    for (LONG i = 0; i < ProductCount; i++)
    {
        auto thirdPartyAVSoftware = parseThirdPartyAVSoftware(ptrProductList, i);
        if (thirdPartyAVSoftware)
        {
            thirdPartyAVSoftwareByName[thirdPartyAVSoftware->Name] = thirdPartyAVSoftware.value();
        }
    }

    //# I have left this case, but its removal could be argued. Its not the same that theres no AV registered
    //# at all, instead of an error. Again this could be easily solved with std::expected
    if (thirdPartyAVSoftwareByName.size() == 0)
    {
        return std::nullopt;
    }
    return thirdPartyAVSoftwareByName;
}