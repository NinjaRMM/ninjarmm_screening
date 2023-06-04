#include "av_software_data_querier.h"

AVSoftwareDataQuerier::AVSoftwareDataQuerier()
{
}

AVSoftwareDataQuerier::~AVSoftwareDataQuerier()
{
}

HRESULT AVSoftwareDataQuerier::createWSCProductList(IWSCProductList** productList)
{
    HRESULT hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(productList));

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create WSCProductList object.");
    }

    return hr;
}

HRESULT AVSoftwareDataQuerier::initializeWSCProductList(IWSCProductList* productList)
{
    HRESULT hr = productList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to query antivirus product list.");
    }

    return hr;
}

HRESULT AVSoftwareDataQuerier::getProductCount(IWSCProductList* productList, LONG* productCount)
{
    HRESULT hr = productList->get_Count(productCount);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to query product count.");
    }

    return hr;
}

void AVSoftwareDataQuerier::releasePtrProduct(IWscProduct* product)
{
    if (product != nullptr) {
        product->Release();
    }
}

void AVSoftwareDataQuerier::releasePtrProductList(IWSCProductList* productList)
{
    if (productList != nullptr) {
        productList->Release();
    }
}

IWSCProductList* AVSoftwareDataQuerier::createAndInitializeWSCProductList()
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

LONG AVSoftwareDataQuerier::getProductsCount(IWSCProductList* ptrProductList)
{
    LONG productCount = 0;
    HRESULT hr = getProductCount(ptrProductList, &productCount);

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to query product count.");
    }

    return productCount;
}

void AVSoftwareDataQuerier::getProductItemFromListAndFillItInAvSoftwareMap(LONG indexInList, IWSCProductList* productList)
{
    try {
        IWscProduct* ptrProduct = nullptr;
        HRESULT hr = productList->get_Item(indexInList, &ptrProduct);

        if (FAILED(hr)) {
            throw std::runtime_error("Failed to query AV product from index " + std::to_string(indexInList));
        }

        std::shared_ptr<AVSoftwareData> avSoftwareData = std::make_shared<AVSoftwareData>(ptrProduct);
        avSoftwareDataMap[avSoftwareData->getName()] = avSoftwareData;
        releasePtrProduct(ptrProduct);
    }
    catch (...) {
        releasePtrProduct(ptrProduct);
    }
}

void AVSoftwareDataQuerier::getProductsData(IWSCProductList* productList, LONG productCount)
{
    for (LONG i = 0; i < productCount; ++i) {
        getProductItemFromListAndFillItInAvSoftwareMap(i, productList);
    }
}

void AVSoftwareDataQuerier::queryAVSoftwareData()
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

const std::map<std::string, std::shared_ptr<AVSoftwareData>>& AVSoftwareDataQuerier::getAVSoftwareDataMap() const
{
    return avSoftwareDataMap;
}
