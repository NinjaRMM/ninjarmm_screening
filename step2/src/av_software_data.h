#include <string>

#pragma once

/**
 * @class AVSoftwareData
 * @brief A class representing an AVSoftwareData entity.
 */
class AVSoftwareData {
public:
    /**
     * @brief Default constructor.
     * @param ptrProduct A pointer to the IWscProduct object.
     * @throws std::runtime_error if failed to query AV product name.
     * @throws std::runtime_error if failed to query AV product state.
     * @throws std::runtime_error if failed to query AV product definition state.
     */
    AVSoftwareData(IWscProduct* ptrProduct);

    /**
     * @brief Getter for the name of the antivirus software.
     * @return The name of the antivirus software.
     */
    std::string getName() const;

    /**
     * @brief Getter for the description of the antivirus software.
     * @return The description of the antivirus software.
     */
    std::string getDescription() const;

    /**
     * @brief Getter for the time of the last definition update.
     * @return The time of the last definition update.
     */
    std::string getDefinitionUpdateTime() const;

    /**
     * @brief Getter for the status of the definition.
     * @return The status of the definition (up-to-date or out-of-date).
     */
    std::string getDefinitionStatus() const;

    /**
     * @brief Getter for the state of the antivirus product.
     * @return The state of the antivirus product (on, off, or expired).
     */
    std::string getProductState() const;

private:
    getDisplayNameFromSystemAPI(IWscProduct* ptrProduct);
    getProductStateFromSystemAPI(IWscProduct* ptrProduct);
    getDefinitionStateFromSystemAPI(IWscProduct* ptrProduct);
    getProductStateTimestampFromSystemAPI(IWscProduct* ptrProduct);

private:
    std::string m_name;                  /**< Name of the antivirus software. */
    std::string m_description;           /**< Description of the antivirus software. */
    std::string m_definitionUpdateTime;  /**< Time of the last definition update. */
    std::string m_definitionStatus;      /**< Status of the definition (up-to-date or out-of-date). */
    std::string m_productState;          /**< State of the antivirus product (on, off, or expired). */
};
