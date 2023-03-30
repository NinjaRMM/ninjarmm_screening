/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/
//added for browsing clarity
#include "ModifiedCodeReview.h"
#include <string>
#include <Windows.h>
#include <map>
#include <iwscapi.h>
#include <wscapi.h>
#include <iostream>
#include <functional>
#include <memory>
#include <atlbase.h>

using namespace std::literals::string_literals;


static ThirdPartyAVSoftware getProductData(CComPtr<IWscProduct>& product);

class QueryWindowsAVSoftware_Error: public std::exception{
public:
    using std::exception::exception;
};

//I prefer to avoid macros as they functionally change the language. However if allowed
//by coding standards I may use them to reduce boiler plate code and facillitate return value checking

#define CHECK_AND_THROW_WINDOWS_AV_QUERY(hres_check, message)\
  if (S_OK != hres_check)\
  {\
      throw QueryWindowsAVSoftware_Error(message);\
  }

#ifndef QUERY_AVSOFTWAREDATA_TESTING //The unit tests will supply their own definition for this
//We wrap this in a seperate function to facilitate unit tests. 
//we can redefine this function during testing to provide a mock object
//We could also create a factory class and supply a derived version of the
//factory class for testing to provide mock objects
CComPtr<IWSCProductList> get_IWSCProductList()
{
    CComPtr<IWSCProductList> product_ListPointer = nullptr;

    HRESULT hr = CoCreateInstance(__uuidof(WSCProductList), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&product_ListPointer));
    CHECK_AND_THROW_WINDOWS_AV_QUERY(hr, "Could not create WSCProductList object. ");
    //to prevent the memory leak use smart ptrs
    return product_ListPointer;
}
#else
CComPtr<IWSCProductList> get_IWSCProductList();
#endif //QUERY_AVSOFTWAREDATA_TESTING

//REVIEW function is fairly long and it is easy to get lost in the the long for loop
bool queryWindowsForAVSoftwareDataWSC_modified(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    HRESULT hr = S_OK;    

    //REVIEW use smart pointers for com objects
    CComPtr<IWSCProductList> PtrProductList;
    LONG ProductCount = 0;
    try {
        //We should have a wrapper around this creation call to facillitate unit testing
       PtrProductList = get_IWSCProductList();

        hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
        CHECK_AND_THROW_WINDOWS_AV_QUERY(hr, "Could not query antivirus product list. ");

        hr = PtrProductList->get_Count(&ProductCount);
        CHECK_AND_THROW_WINDOWS_AV_QUERY(hr, "Could not query product count.");
    }
    catch (QueryWindowsAVSoftware_Error& e) {
        //REVIEW consider using exceptions to reduce the amount of redundant error calls;
        //when combined with smart pointers  we can be sure the memory destruction is properly handled
        std::cerr << e.what() << std::endl;
        return false;
    }

    for (int i = 0; i < ProductCount; i++)
    {
        //REVIEW put the majority of the code in this for loop in the subfunction getProductData
        //This keeps the goal of the overal function queryWindowsForAVSoftwareDataWSC clear
        try {
            CComPtr<IWscProduct> ptrProduct = nullptr;
            hr = PtrProductList->get_Item(i, &ptrProduct);
            CHECK_AND_THROW_WINDOWS_AV_QUERY(hr, "Could not query AV product.");

            ThirdPartyAVSoftware thirdPartyAVSoftware = getProductData(ptrProduct);

            //REVIEW since this map is passed in by reference we are potentionally overriding existing data. is this intentional?
            thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
        }
        catch (QueryWindowsAVSoftware_Error& e) {
            //Use exceptions to catch all the various failure points and have one continue statement
            std::cerr << e.what() << std::endl;
            continue;
        }
    }

    //REVIEW use empty
    if (thirdPartyAVSoftwareMap.empty())
    {
        return false;
    }
    return true;
}

//The following templates determine the first argument to a member function.
// used for making generic lambdas based on the type of the first argument
// i.e. first_arg_t<decltype(&string::resize)> == size_t
// 
//Inspiration from stackoverflow answer https://stackoverflow.com/a/70954691
//Specialized it to handle member functions and added the first_arg_t using declaration for ease of use.
//Specialized it to only return the type of the first arg with the pointer removed
//Also removed the needless (for my purposes) undefined function in the original answer
template<typename Sig>
struct first_arg;

//Specialization to break apart the types in a member function type.
template<typename R, typename T, typename ...Args>
struct first_arg<R (T::*)(Args...)>
{
    //we make a tuple and save the first type
    using type = std::tuple_element_t<0, std::tuple<Args...>>;
};

//Removes the ptr from the first arg type so we can declare it locally
template<typename MemFuncPtr>
using first_arg_t = std::remove_pointer_t<typename first_arg<MemFuncPtr>::type>;



//REVIEW make this a seprate function to reduce line count and complexity of getProductData function
static std::wstring get_product_state_string(WSC_SECURITY_PRODUCT_STATE productState) {
    switch (productState)
    {
    case WSC_SECURITY_PRODUCT_STATE_ON:
        return L"On";
    case WSC_SECURITY_PRODUCT_STATE_OFF:
        return L"Off";
    case WSC_SECURITY_PRODUCT_STATE_SNOOZED:
        //REVIEW: this enum was not handled. it defaulted to expired. unsure if this was intentional
        return L"Snoozed";
    case WSC_SECURITY_PRODUCT_STATE_EXPIRED:
        return L"Expired";
    default:
        //REVIEW: what should happen if we get a garbage value for productState?
        throw QueryWindowsAVSoftware_Error("Could not parse product state");
        return L"Unknown";
    }
}

//Gets the product data from a IWscProduct and returns a ThirdPartyAVSoftware
ThirdPartyAVSoftware getProductData(CComPtr<IWscProduct>& ptrProduct) {

    HRESULT hr = S_OK;
    ThirdPartyAVSoftware thirdPartyAVSoftware{};

    //Takes a IWscProduct and a memberfunction that outputs a BTSR* to the first argument
    // and returns a wstring of that BTSR*. Then releases the BTSR resource
    auto get_wstring_from_product = [](
                            CComPtr<IWscProduct>& l_product
                            , auto member_function
                            , const char* message_on_error) -> std::wstring
                    {
                        //REVIEW use smart ptr to avoid memory leaks
                        CComBSTR byte_str_ptr;
                        HRESULT hres = (l_product->*member_function)(&byte_str_ptr);
                        CHECK_AND_THROW_WINDOWS_AV_QUERY(hres, message_on_error);
                        //bytestrings may have nulls in them. so we use this constructor
                        //to take the whole string
                        std::wstring value_str(byte_str_ptr, SysStringLen(byte_str_ptr));
                        //std::move ensures a move but may prevent ellision 
                        return value_str;
                    };
    
    //Takes a IWscProduct and a memberfunction that outputs enum data to the first argument
    // and returns that enum.
    auto get_enum_from_product = [](
                            CComPtr<IWscProduct>& l_product
                            , auto member_function
                            , const char* message_on_error)
                            ->first_arg_t<decltype(member_function)>
                        {

                            first_arg_t<decltype(member_function)> value;
                            HRESULT hres = (l_product->*member_function)(&value);
                            CHECK_AND_THROW_WINDOWS_AV_QUERY(hres, message_on_error);
                            return value;

                        };

                        
    thirdPartyAVSoftware.Name = get_wstring_from_product(ptrProduct, &IWscProduct::get_ProductName, "Could not query product name");
    
    WSC_SECURITY_PRODUCT_STATE productState 
        = get_enum_from_product(ptrProduct, &IWscProduct::get_ProductState, "Could not query product state");
    
    thirdPartyAVSoftware.ProductState = get_product_state_string(productState);

    WSC_SECURITY_SIGNATURE_STATUS productStatus
        = get_enum_from_product(ptrProduct, &IWscProduct::get_SignatureStatus, "Could not query AV product definition state");


    thirdPartyAVSoftware.DefinitionStatus 
        = (productStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? L"UpToDate" : L"OutOfDate";

        
    //REVIEW original code had the wrong error message on failure to get productstatetimestamp info
    thirdPartyAVSoftware.DefinitionUpdateTime
        = get_wstring_from_product(ptrProduct, &IWscProduct::get_ProductStateTimestamp, "Could not query product state timestamp");

   
    //REVIEW did description also mean to be set to productState??
    //thirdPartyAVSoftware.Description = thirdPartyAVSoftware.ProductState;

    //compilers may naturally std::move this.
    //we don't use std::move so we don't preven ellision
    return thirdPartyAVSoftware;
}
