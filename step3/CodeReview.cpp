/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

#include <cassert>
#include <iostream>
#include <map>
#include <string>

#include <Windows.h>
#include <iwscapi.h>
#include <wscapi.h>

//#include "src/CodeReview.h" // uncomment to use header instead

#ifndef CODEREVIEW_H
struct ThirdPartyAVSoftware // move to a header, See src/CodeReview.h
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version; /* unused */
    std::wstring ProductState;
};
#endif

class Product
{
private:
    IWscProduct* raw; // RAII
    bool isGood;

    void GetName(std::wstring& name)
    {
        assert(isGood);

        auto PtrVal = BSTR(nullptr);
        auto hr = raw->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product name.";
            isGood = false;
            return;
        }

        assert(PtrVal != nullptr);
        name = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);
        PtrVal = nullptr;
    }

    void GetState(std::wstring& state)
    {
        assert(isGood);

        auto ProductState = WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_EXPIRED;
        auto hr = raw->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product state.";
            isGood = false;
            return;
        }

        switch (ProductState)
        {
        case WSC_SECURITY_PRODUCT_STATE_ON:
            state = L"On";
            break;
        case WSC_SECURITY_PRODUCT_STATE_OFF:
            state = L"Off";
            break;
        default:
            // WSC_SECURITY_PRODUCT_STATE_SNOOZED - is this intentional?
            // WSC_SECURITY_PRODUCT_STATE_EXPIRED
            state = L"Expired";
            break;
        }
    }

    void GetDefinitionStatus(std::string& status)
    {
        assert(isGood);

        auto ProductStatus = WSC_SECURITY_SIGNATURE_STATUS::WSC_SECURITY_PRODUCT_OUT_OF_DATE;
        auto hr = raw->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            isGood = false;
            return;
        }

        status = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ?
            std::string("UpToDate") : std::string("OutOfDate");
    }

    void GetDefinitionUpdateTime(std::wstring& time)
    {
        assert(isGood);

        auto PtrVal = BSTR(nullptr);
        auto hr = raw->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product definition state.";
            isGood = false;
            return;
        }

        assert(PtrVal != nullptr);
        time = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);
        PtrVal = nullptr;
    }

public:
    Product(IWscProduct* product) : raw(product), isGood(true) {}

    ~Product()
    {
        assert(isGood);
        raw->Release();
    }

    void GetInfo(ThirdPartyAVSoftware& software)
    {
        assert(isGood);

        if (isGood)
        {
            GetName(software.Name);
        }

        if (isGood)
        {
            GetState(software.ProductState);
        }
        software.Description = software.ProductState; // Is this intentional?

        if (isGood)
        {
            GetDefinitionStatus(software.DefinitionStatus);
        }

        if (isGood)
        {
            GetDefinitionUpdateTime(software.DefinitionUpdateTime);
        }
    }

    bool IsGood() const { return isGood; }
};

class ProductList
{
private:
    IWSCProductList* raw; // RAII
    long count;
    bool isGood;

    void CreateInstance()
    {
        auto hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&raw));
        if (FAILED(hr))
        {
            std::cout << "Failed to create WSCProductList object. ";
            isGood = false;
        }

        isGood = true;
    }

    void Init()
    {
        assert(isGood);

        auto hr = raw->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
        if (FAILED(hr))
        {
            std::cout << "Failed to query antivirus product list. ";
            isGood = false;
        }
    }

    void GetTotalCount()
    {
        assert(isGood);

        auto hr = raw->get_Count(&count);
        if (FAILED(hr))
        {
            std::cout << "Failed to query product count.";
            isGood = false;
        }
    }

public:
    ProductList() : raw(nullptr), count(-1), isGood(true)
    {
        CreateInstance();

        if (isGood)
        {
            Init();
        }

        if (isGood)
        {
            GetTotalCount();
        }
    }

    std::unique_ptr<Product> GetProduct(long i)
    {
        assert(isGood);

        auto product = (IWscProduct*)nullptr;
        auto hr = raw->get_Item(i, &product);
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            isGood = false;
            return nullptr;
        }

        return std::make_unique<Product>(product);
    }

    long GetCount() const { return count; }
    bool IsGood() const { return isGood; }
};

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
    auto productList = std::make_unique<ProductList>();
    if (!productList->IsGood())
    {
        return false;
    }

    for (auto i = 0L; i < productList->GetCount(); ++i)
    {
        ThirdPartyAVSoftware thirdPartyAVSoftware;
        auto product = productList->GetProduct(i);
        
        if (product != nullptr)
        {
            product->GetInfo(thirdPartyAVSoftware);
            if (product->IsGood())
            {
                thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
            }
        }
    }

    return (thirdPartyAVSoftwareMap.size() > 0);
}
