#include "av_software_data.h"

AVSoftwareData::AVSoftwareData()
{
}

AVSoftwareData::~AVSoftwareData()
{
}

HRESULT AVSoftwareData::createWSCProductList(IWSCProductList** productList)
{
    HRESULT hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(productList));

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create WSCProductList object.");
    }

    return hr;
}

HRESULT AVSoftwareData::initializeWSCProductList(IWSCProductList* productList)
{
    HRESULT hr = productList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to query antivirus product list.");
    }

    return hr;
}

HRESULT AVSoftwareData::getProductCount(IWSCProductList* productList, LONG* productCount)
{
    HRESULT hr = productList->get_Count(productCount);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to query product count.");
    }

    return hr;
}

std::string AVSoftwareData::getDisplayName(IWscProduct* product)
{
    std::string displayName;
    BSTR ptrVal = nullptr;
    HRESULT hr = product->get_ProductName(&ptrVal);

    if (SUCCEEDED(hr)) {
        displayName = std::string(ptrVal, SysStringLen(ptrVal));
        SysFreeString(ptrVal);
    } else {
        throw std::runtime_error("Failed to query AV product name.");
    }

    return displayName;
}

std::string AVSoftwareData::getProductState(IWscProduct* product)
{
    std::string state;
    WSC_SECURITY_PRODUCT_STATE productState;
    HRESULT hr = product->get_ProductState(&productState);

    if (SUCCEEDED(hr)) {
        if (productState == WSC_SECURITY_PRODUCT_STATE_ON) {
            state = "On";
        } else if (productState == WSC_SECURITY_PRODUCT_STATE_OFF) {
            state = "Off";
        } else {
            state = "Expired";
        }
    } else {
        throw std::runtime_error("Failed to query AV product state.");
    }
    return state;
}

std::string AVSoftwareData::getDefinitionState(IWscProduct* product)
{
    std::string definitionState;
    WSC_SECURITY_SIGNATURE_STATUS productStatus;
    HRESULT hr = product->get_SignatureStatus(&productStatus);

    if (SUCCEEDED(hr)) {
        definitionState = (productStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";
    } else {
        throw std::runtime_error("Failed to query AV product definition state.");
    }

    return definitionState;
}

std::string AVSoftwareData::getProductStateTimestamp(IWscProduct* product)
{
    std::string timestamp;
    BSTR ptrVal = nullptr;
    HRESULT hr = product->get_ProductStateTimestamp(&ptrVal);

    if (SUCCEEDED(hr)) {
        timestamp = std::string(ptrVal, SysStringLen(ptrVal));
        SysFreeString(ptrVal);
    } else {
        throw std::runtime_error("Failed to query AV product definition state.");
    }

    return timestamp;
}

void AVSoftwareData::processAVProduct(IWscProduct* product)
{
    std::string displayName = getDisplayName(product);
    std::string state = getProductState(product);
    std::string definitionState = getDefinitionState(product);
    std::string timestamp = getProductStateTimestamp(product);

    ThirdPartyAVSoftware thirdPartyAVSoftware;
    thirdPartyAVSoftware.name = displayName;
    thirdPartyAVSoftware.definitionStatus = definitionState;
    thirdPartyAVSoftware.definitionUpdateTime = timestamp;
    thirdPartyAVSoftware.description = state;
    thirdPartyAVSoftware.productState = state;

    thirdPartyAVSoftwareMap[thirdPartyAVSoftware.name] = thirdPartyAVSoftware;
}

void AVSoftwareData::releasePtrProduct(IWscProduct* product)
{
    if (product != nullptr) {
        product->Release();
    }
}

void AVSoftwareData::releasePtrProductList(IWSCProductList* productList)
{
    if (productList != nullptr) {
        productList->Release();
    }
}

IWSCProductList* AVSoftwareData::createAndInitializeWSCProductList()
{
    IWSCProductList* ptrProductList = nullptr;
    HRESULT hr = createWSCProductList(&ptrProductList);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create WSCProductList object.");
    }

    hr = initializeWSCProductList(ptrProductList);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to query antivirus product list.");
    }

    return ptrProductList;
}

LONG AVSoftwareData::getProductsCount(IWSCProductList* ptrProductList)
{
    LONG productCount = 0;
    HRESULT hr = getProductCount(ptrProductList, &productCount);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to query product count.");
    }

    return productCount;
}

void AVSoftwareData::getProductItemFromListAndFillItInAvSoftwareMap(LONG indexInList, IWSCProductList* productList)
{
    try {
        IWscProduct* ptrProduct = nullptr;
        HRESULT hr = productList->get_Item(indexInList, &ptrProduct);

        if (FAILED(hr)) {
            throw std::runtime_error("Failed to query AV product from index " + std::to_string(indexInList));
        }

        processAVProduct(ptrProduct);
    }
    catch (...) {
        releasePtrProduct(ptrProduct);
    }
}

void AVSoftwareData::getProductsData(IWSCProductList* productList, LONG productCount)
{
    for (LONG i = 0; i < productCount; ++i) {
        getProductItemFromListAndFillItInAvSoftwareMap(i, productList);
    }
}

void AVSoftwareData::queryAVSoftwareData()
{
    IWSCProductList* ptrProductList = nullptr;

    try {
        ptrProductList = createAndInitializeWSCProductList();
        LONG productCount = getProductsCount(ptrProductList);
        getProductsData(ptrProductList, productCount);
        releasePtrProductList(ptrProductList);
    } catch (...) {
        releasePtrProductList(ptrProductList);
        throw;
    }
}

const std::map<std::string, ThirdPartyAVSoftware>& AVSoftwareData::getAVSoftwareMap() const
{
    return thirdPartyAVSoftwareMap;
}
