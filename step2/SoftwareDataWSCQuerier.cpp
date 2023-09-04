#include "SoftwareDataWSCQuerier.h"


// <<<<<<<<<<<<<<< SoftwareDataWSCQueryBuilder

void SoftwareDataWSCQueryBuilder::IncrementProductIndex() {
    productIndex = (productIndex == std::numeric_limits<LONG>::max()) ? 0L : productIndex + 1;
}

bool SoftwareDataWSCQueryBuilder::HasProductsToLoad()
{
    return (ProductCount > 0)
        && (productIndex == std::numeric_limits<LONG>::max() || productIndex + 1 < ProductCount);
}

bool SoftwareDataWSCQueryBuilder::AnythingBuilt()
{
    return thirdPartyWSCSoftwareMap.size() > 0;
}


// <<<<<<<<<<<<<<< AntivirusSoftwareDataWSCQueryBuilder

void AntivirusSoftwareDataWSCQueryBuilder::InitializeList() 
{
    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID *>(&PtrProductList));
    if (FAILED(hr))
    {
        throw WSCListError(WSCError::LIST_CREATION_ERRROR);
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        throw WSCListError(WSCError::AV_PRODUCT_LIST_QUERY_ERROR);
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        throw WSCListError(WSCError::PRODUCT_COUNT_QUERY_ERROR);
    }
}

bool AntivirusSoftwareDataWSCQueryBuilder::LoadNextProduct()
{
    IncrementProductIndex();

    hr = PtrProductList->get_Item(productIndex, &PtrProduct);
    if (FAILED(hr))
    {
        throw WSCQueryError(WSCError::AV_PRODUCT_QUERY_ERROR);
    }

    return productIndex < ProductCount;
}

void AntivirusSoftwareDataWSCQueryBuilder::ReadProductName()
{
    hr = PtrProduct->get_ProductName(&PtrVal);
    if (FAILED(hr))
    {
        PtrProduct->Release();
        throw WSCQueryError(WSCError::AV_PRODUCT_QUERY_NAME_ERROR);
    }

    displayName = std::wstring(PtrVal, SysStringLen(PtrVal));
}

void AntivirusSoftwareDataWSCQueryBuilder::ReadProductState()
{
    hr = PtrProduct->get_ProductState(&ProductState);
    if (FAILED(hr))
    {
        throw WSCQueryError(WSCError::AV_PRODUCT_QUERY_STATE_ERROR);
    }

    state = ProductStateMapper::Get(ProductState);
}

void AntivirusSoftwareDataWSCQueryBuilder::ReadSignatureStatus()
{
    hr = PtrProduct->get_SignatureStatus(&ProductStatus);
    if (FAILED(hr))
    {
        throw WSCQueryError(WSCError::AV_PRODUCT_QUERY_DEFINITION_STATE_ERROR);
    }

    definitionState = DefinitionStateMapper::Get(ProductStatus);
}

void AntivirusSoftwareDataWSCQueryBuilder::ReadProductStateTimestamp()
{
    hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
    if (FAILED(hr))
    {
        throw WSCQueryError(WSCError::AV_PRODUCT_QUERY_DEFINITION_STATE_ERROR);
    }
    timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
    SysFreeString(PtrVal);

}

void AntivirusSoftwareDataWSCQueryBuilder::ReleaseProduct()
{
    PtrProduct->Release();
}

void AntivirusSoftwareDataWSCQueryBuilder::UpdateContainer()
{
    ThirdPartyWSCSoftware thirdPartyWSCSoftware;
    thirdPartyWSCSoftware.Name = displayName;
    thirdPartyWSCSoftware.DefinitionStatus = definitionState;
    thirdPartyWSCSoftware.DefinitionUpdateTime = timestamp;
    thirdPartyWSCSoftware.Description = state;
    thirdPartyWSCSoftware.ProductState = state;
    thirdPartyWSCSoftwareMap[thirdPartyWSCSoftware.Name] = thirdPartyWSCSoftware;
}


// <<<<<<<<<<<<<<< SoftwareDataWSCQuerier

void SoftwareDataWSCQuerier::QueryWindowsForSoftwareDataWSC(SoftwareDataWSCQueryBuilder& builder)
{
    try
    {
        builder.InitializeList();
    }
    catch(const WSCListError& e)
    {
        std::cout << e.what();
        return;
    }

    while(builder.HasProductsToLoad())
    {
        bool loaded;
        try
        {
            loaded = builder.LoadNextProduct();
            builder.ReadProductName();
            builder.ReadProductState();
            builder.ReadSignatureStatus();
            builder.ReadProductStateTimestamp();
        }
        catch(const WSCQueryError& e) // keep the loop running
        {
            std::cout << e.what(); 
        }

        if (loaded)
            builder.ReleaseProduct();
    }
}