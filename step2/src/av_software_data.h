#include <map>
#include <string>
#include <stdexcept>

#pragma once

/**
 * @brief Structure representing the third-party antivirus software information.
 */
struct ThirdPartyAVSoftware
{
    std::string name;                   /**< Name of the antivirus software. */
    std::string description;            /**< Description of the antivirus software. */
    std::string definitionUpdateTime;   /**< Time of the last definition update. */
    std::string definitionStatus;       /**< Status of the definition (up-to-date or out-of-date). */
    std::string productState;           /**< State of the antivirus product (on, off, or expired). */
};

/**
 * @brief Class for retrieving antivirus software information using the Windows API.
 */
class AVSoftwareData
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
    const std::map<std::string, ThirdPartyAVSoftware>& getAVSoftwareMap() const;

private:
    // Helper methods
    HRESULT createWSCProductList(IWSCProductList** productList);
    HRESULT initializeWSCProductList(IWSCProductList* productList);
    HRESULT getProductCount(IWSCProductList* productList, LONG* productCount);
    std::string getDisplayName(IWscProduct* product);
    std::string getProductState(IWscProduct* product);
    std::string getDefinitionState(IWscProduct* product);
    std::string getProductStateTimestamp(IWscProduct* product);
    void processAVProduct(IWscProduct* product);
    void releasePtrProduct(IWscProduct* product);
    void releasePtrProductList(IWSCProductList* productList);
    IWSCProductList* createAndInitializeWSCProductList();
    LONG getProductsCount(IWSCProductList* ptrProductList);
    void getProductItemFromListAndFillItInAvSoftwareMap(LONG indexInList, IWSCProductList* productList);
    void getProductsData(IWSCProductList* productList, LONG productCount);

private:
    std::map<std::string, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap; /**< Map containing the third-party antivirus software information. */

};
