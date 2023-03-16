//
// I run this program using VS2019 C++14 Standard
//

#define TEST
#ifdef TEST
    #include <iostream>
    #include <sstream> 
    #include <ios>
    #include <map>
    #include <exception> // for std::exception
    #include <stdexcept> // for std::runtime_error 
    #include <windows.h>
    #include <iwscapi.h>
    #include <wscapi.h>
#endif

//
// This class is responsible for an automatic release when the comPointer exit scope,
// in addition it simplify the CoCreateInstance API signiture when applicable to reduce the programmer typo
// resulting in mismatch between CLSID and interface
// NOTE- This is a partial implementation relevant for this exercise only in real world refer to 
// com standard smart pointers see #include <atlbase.h>.
//
template <typename T>
class MySmartComPointer
{
    T* m_ptr;
public:
    MySmartComPointer() : m_ptr(nullptr) {}
    MySmartComPointer(T* p) : m_ptr(p) {  }
    ~MySmartComPointer() { if (m_ptr) { m_ptr->Release(); m_ptr = nullptr; } }
    T* operator->() const { return m_ptr; }
    T** operator&() { return &m_ptr; }
    HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext) {
        return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, IID_PPV_ARGS(&m_ptr));
    }
};

//
// This class is responsible to automatic release a BSTR on scope exit and handle BSTR plumping
//
class MySmartBSTR
{
    BSTR m_bstr;
public:
    MySmartBSTR() : m_bstr(nullptr) {}
    MySmartBSTR(BSTR p) : m_bstr(p) {  }
    ~MySmartBSTR() { if (SysStringLen(m_bstr) > 0) { SysFreeString(m_bstr); m_bstr = nullptr;} }
    std::wstring ConvertBstrToWString() { std::wstring wstr(m_bstr, SysStringLen(m_bstr)); return wstr; }
    BSTR operator->() const { return m_bstr; }
    BSTR * operator&() { return &m_bstr; }
};

//
// This function is responsible to check if last result failed, on fail print error description
// and throw a run time exception
// TBD- use com_error exception
//
inline void ThrowExceptionIfFail(HRESULT hr, std::string&& description)
{
    if (FAILED(hr))
    {
        //std::ios_base::fmtflags f(std::cout.flags());
        std::stringstream ss;
        ss << description << " Error code: 0x" << std::hex << hr << std::endl;
        //std::cout.flags(f);
        throw std::runtime_error(ss.str());
    }
}

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::wstring DefinitionStatus;   // make wide to be consistent with all types
    std::wstring Version;            // for future useage, currently not populated
    std::wstring ProductState;
};

//
// --------------------------------------------------------------------------------------------------------
// Code review main observations(pre optimization)
// --------------------------------------------------------------------------------------------------------
// 1.PtrProductList was not released
// 2.CoCreateInstance for PtrProductLine was at risk for mismatch between clsid and interface
// 3.fail msg missing hr, further more fail msg for get_SignatureStatus not distinct(similar
//   to get_ProductStateTimestamp
// 4.ProductState was missing handling for the snooze case
// 5.mismatch types in for (uint32_t i = 0; i < ProductCount; i++) between uint32_t and ProductCount(ULONG)
// 6.BSTR PtrVal not released after wsting converstion to DisplayName
// 7.Version not populated
// 8.DefinitionStaus is the only non wide string
// 9.The stack is overloadded with temporary variable which are assigned to thirdPartyAVSoftwareMap(seems redundent)
// 10.variable are defined at top of the function not in the proximity of usage
// 11.Error code pattern is repeated
// 12.BSTR pluming are repeated(convert to wstring, release etc)
// ---------------------------------------------------------------------------------------------------------
// Optimization strategy
// ----------------------------------------------------------------------------------------------------------
// 1.Use smartPointers to manage ComPointers & BSTR (safeRelease)
// 2.Reduce code via code reuse(BSTR pluming , Error handling)
// 3.Reduce temporary variables, assign directly to thirdPartyAVSoftwareMap when applicable
// 4.Improve code readability by declaring variables in proximity to usage
// 5.Add missing case handling/ fix type mismatch 
// ---------------------------------------------------------------------------------------------------------
// Known pitfalls(future optimization candidates)
// ---------------------------------------------------------------------------------------------------------
// 1.ThrowExceptionIfFail allocates string decoupled from fail condition
// 2.On Product error handling inside the loop we continue on error but throw an exception(costly)hence if fail frequency is above a certain
// threshold(TBD) the trade off between cleaner code(exception handling) vs code optimization (cascade fail pattern) may favor the code optimization.
//
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    try
    {
        //
        // Make PtrProductList smart to enforce automatic release and intialization...
        //
        MySmartComPointer<IWSCProductList> PtrProductList;

        ThrowExceptionIfFail(PtrProductList.CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER),
            "Failed to create WSCProductList object.");
       
        ThrowExceptionIfFail(PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS),
            "Failed to query antivirus product list.");
        
        LONG ProductCount = 0;
        ThrowExceptionIfFail(PtrProductList->get_Count(&ProductCount), 
            "Failed to query product count.");
        
        WSC_SECURITY_PRODUCT_STATE ProductState;
        WSC_SECURITY_SIGNATURE_STATUS ProductStatus;
        for (LONG i = 0; i < ProductCount; i++)
        {
            try
            {
                //
                // Reset new map item
                //
                ThirdPartyAVSoftware thirdPartyAVSoftware = {};

                //
                // Make smart pointer to ensure automatic release and intialization
                //
                MySmartComPointer<IWscProduct> PtrProduct;
                ThrowExceptionIfFail(PtrProductList->get_Item(i, &PtrProduct), 
                    "Failed to query AV product.");
                
                // 
                // limit BSTR scope ...
                //
                {
                    MySmartBSTR PtrVal;
                    ThrowExceptionIfFail(PtrProduct->get_ProductName(&PtrVal), 
                        "Failed to query AV product name.");

                    thirdPartyAVSoftware.Name = PtrVal.ConvertBstrToWString();
                    // BSTR automatic disposal...
                }

                ThrowExceptionIfFail(PtrProduct->get_ProductState(&ProductState), 
                    "Failed to query AV product state.");
                
                if (ProductState == WSC_SECURITY_PRODUCT_STATE_ON)
                {
                    thirdPartyAVSoftware.ProductState = L"On";
                }
                else if (ProductState == WSC_SECURITY_PRODUCT_STATE_OFF)
                {
                    thirdPartyAVSoftware.ProductState = L"Off";
                }
                // add missing case
                else if (ProductState == WSC_SECURITY_PRODUCT_STATE_SNOOZED)
                {
                    thirdPartyAVSoftware.ProductState = L"Snoozed";;
                }
                else
                {
                    thirdPartyAVSoftware.ProductState = L"Expired";
                }

                ThrowExceptionIfFail(PtrProduct->get_SignatureStatus(&ProductStatus),
                    "Failed to query AV product definition state.");

                thirdPartyAVSoftware.DefinitionStatus = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? L"UpToDate" : L"OutOfDate";

                // 
                // limit BSTR scope ...
                //
                {
                    MySmartBSTR PtrVal;
                    ThrowExceptionIfFail(PtrProduct->get_ProductStateTimestamp(&PtrVal),
                        "Failed to query AV product definition state timestamp.");;
                    thirdPartyAVSoftware.DefinitionUpdateTime = PtrVal.ConvertBstrToWString();
                    // BSTR automatic disposal...
                }

                // 
                //not sure why description should match state ?? but whatever..
                //
                thirdPartyAVSoftware.Description = thirdPartyAVSoftware.ProductState;

                //
                // add item to map on happy path
                //
                thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
            }
            // on sad path during loop continue
            catch (const std::exception& exception)
            {
                std::cout << exception.what();
                continue;
            }

            //PtrProduct automatic release...
        }
        if (thirdPartyAVSoftwareMap.size() == 0)
        {
            return false;
        }
        return true;
    }
    //on sad path before loop abort 
    catch (const std::exception& exception)
    {
        std::cout << exception.what();
        return false;
    }
    
    // PtrProductList automatic release...
}

#ifdef TEST
int main()
{
    HRESULT hr = S_OK;
    std::map<std::wstring, ThirdPartyAVSoftware> myMap;
    hr= CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        if (queryWindowsForAVSoftwareDataWSC(myMap))
        {
            for (const auto& item : myMap) 
            {
                std::wcout << "[Name:]"<< item.first << std::endl;
                std::wcout << "->Name:"<<item.second.Name << std::endl;
                std::wcout << "->Desc:"<<item.second.Description << std::endl;
                std::wcout << "->Status:"<<item.second.DefinitionStatus << std::endl;
                std::wcout << "->Time:"<<item.second.DefinitionUpdateTime << std::endl;
                std::wcout << "->State:"<< item.second.ProductState << std::endl;
                std::wcout << "->Ver:" << item.second.Version << std::endl;
            }
        }
        CoUninitialize();
    }
}
#endif

