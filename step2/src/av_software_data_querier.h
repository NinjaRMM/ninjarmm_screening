#include <map>
#include <string>
#include <memory>
#include <stdexcept>

#include "av_software_data.h"

#pragma once

/**
 * @brief Class for retrieving antivirus software information using the Windows API.
 */
class AVSoftwareDataQuerier
{
public:
    /**
     * @brief Queries the Windows API for antivirus software data.
     * @throw std::runtime_error if an error occurs during the data retrieval process.
     */
    void queryAVSoftwareData();

    /**
     * @brief Gets the map of third-party antivirus software.
     * @return const reference to the map of third-party antivirus software.
     */
    const std::map<std::string, std::shared_ptr<AVSoftwareData>>& getAVSoftwareDataMap() const;

private:
    HRESULT createWSCProductList(IWSCProductList** productList);
    HRESULT initializeWSCProductList(IWSCProductList* productList);
    HRESULT getProductCount(IWSCProductList* productList, LONG* productCount);
    void releasePtrProduct(IWscProduct* product);
    void releasePtrProductList(IWSCProductList* productList);
    IWSCProductList* createAndInitializeWSCProductList();
    LONG getProductsCount(IWSCProductList* ptrProductList);
    void getProductItemFromListAndFillItInAvSoftwareMap(LONG indexInList, IWSCProductList* productList);
    void getProductsData(IWSCProductList* productList, LONG productCount);

private:
    std::map<std::string, std::shared_ptr<AVSoftwareData>> avSoftwareDataMap; /**< Map containing the third-party antivirus software information. */
};
