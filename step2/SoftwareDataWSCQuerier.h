#pragma once

#include <string>
#include <map>
#include <limits>
#include "Win32APIEnumsAnsTypes.h"
#include "Mappers.h"
#include "WSCError.h"

/*
    The classes in this file implement the pattern Builder, where
        > SoftwareDataWSCQuerier               is a Director
        > SoftwareDataWSCQueryBuilder          is an abstract builder 
        > AntivirusSoftwareDataWSCQueryBuilder is a concrete builder
      
        > Obs: AntiSpywareSoftwareDataWSCQueryBuilder, FirewallSoftwareDataWSCQueryBuilder, ...
        > ... could be other concrete builders too (if they are relevant to the application).
*/

struct ThirdPartyWSCSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

// Abstract builder
class SoftwareDataWSCQueryBuilder
{
protected:
    LONG ProductCount = 0;
    LONG productIndex = std::numeric_limits<LONG>::max(); // added to iterate over products
    std::map<std::wstring, ThirdPartyWSCSoftware> thirdPartyWSCSoftwareMap;

    void IncrementProductIndex(); // added to iterate over products

public:

    SoftwareDataWSCQueryBuilder(std::map<std::wstring, ThirdPartyWSCSoftware> &_thirdPartyWSCSoftwareMap)
        : thirdPartyWSCSoftwareMap(_thirdPartyWSCSoftwareMap) {}
        
    virtual ~SoftwareDataWSCQueryBuilder() = default;

    virtual void InitializeList() = 0;

    virtual bool LoadNextProduct() = 0;

    virtual void ReadProductName() = 0;

    virtual void ReadProductState() = 0;

    virtual void ReadSignatureStatus() = 0;

    virtual void ReadProductStateTimestamp() = 0;

    virtual void ReleaseProduct() = 0;

    virtual void UpdateContainer() = 0;

    bool HasProductsToLoad();

    bool AnythingBuilt();
    
};

// Concrete builder
class AntivirusSoftwareDataWSCQueryBuilder : public SoftwareDataWSCQueryBuilder
{
private:
    HRESULT hr = S_OK;
    IWscProduct *PtrProduct = nullptr;
    IWSCProductList *PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    std::wstring displayName, versionNumber, state, timestamp;
    std::string definitionState;
    
public:

    AntivirusSoftwareDataWSCQueryBuilder(std::map<std::wstring, ThirdPartyWSCSoftware> &_thirdPartyWSCSoftwareMap)
        : SoftwareDataWSCQueryBuilder(_thirdPartyWSCSoftwareMap) { }
    ~AntivirusSoftwareDataWSCQueryBuilder() = default;

    void InitializeList() override ;

    bool LoadNextProduct() override;

    void ReadProductName() override;

    void ReadProductState() override;
    
    void ReadSignatureStatus() override;
    
    void ReadProductStateTimestamp() override;
    
    void ReleaseProduct() override;
    
    void UpdateContainer() override;

};

// Director
class SoftwareDataWSCQuerier
{
public:
    SoftwareDataWSCQuerier() = default;
    ~SoftwareDataWSCQuerier() = default;

    void QueryWindowsForSoftwareDataWSC(SoftwareDataWSCQueryBuilder& builder);
};