#include "av_software_data.h"

AVSoftwareData::AVSoftwareData(IWscProduct* ptrProduct)
{
    getDisplayNameFromSystemAPI(ptrProduct);
    getProductStateFromSystemAPI(ptrProduct);
    getDefinitionStateFromSystemAPI(ptrProduct);
    getProductStateTimestampFromSystemAPI(ptrProduct);
}

std::string AVSoftwareData::getName() const
{
    return m_name;
}

std::string AVSoftwareData::getDescription() const
{
    return m_description;
}

std::string AVSoftwareData::getDefinitionUpdateTime() const
{
    return m_definitionUpdateTime;
}

std::string AVSoftwareData::getDefinitionStatus() const
{
    return m_definitionStatus;
}

std::string AVSoftwareData::getProductState() const
{
    return m_productState;
}

void AVSoftwareData::getDisplayNameFromSystemAPI(IWscProduct* ptrProduct)
{
    BSTR ptrVal = nullptr;
    HRESULT hr = ptrProduct->get_ProductName(&ptrVal);

    if (SUCCEEDED(hr)) {
        m_name = std::string(ptrVal, SysStringLen(ptrVal));
        SysFreeString(ptrVal);
    } else {
        throw std::runtime_error("Failed to query AV product name.");
    }
}

void AVSoftwareData::getProductStateFromSystemAPI(IWscProduct* ptrProduct)
{
    WSC_SECURITY_PRODUCT_STATE productState;
    HRESULT hr = ptrProduct->get_ProductState(&productState);

    if (SUCCEEDED(hr)) {
        if (productState == WSC_SECURITY_PRODUCT_STATE_ON) {
            m_productState = "On";
        } else if (productState == WSC_SECURITY_PRODUCT_STATE_OFF) {
            m_productState = "Off";
        } else {
            m_productState = "Expired";
        }
        m_description = m_productState;
    } else {
        throw std::runtime_error("Failed to query AV product state.");
    }
}

void AVSoftwareData::getDefinitionStateFromSystemAPI(IWscProduct* ptrProduct)
{
    WSC_SECURITY_SIGNATURE_STATUS productStatus;
    HRESULT hr = ptrProduct->get_SignatureStatus(&productStatus);

    if (SUCCEEDED(hr)) {
        m_definitionStatus = (productStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";
    } else {
        throw std::runtime_error("Failed to query AV product definition state.");
    }
}

void AVSoftwareData::getProductStateTimestampFromSystemAPI(IWscProduct* ptrProduct)
{
    BSTR ptrVal = nullptr;
    HRESULT hr = ptrProduct->get_ProductStateTimestamp(&ptrVal);

    if (SUCCEEDED(hr)) {
        m_definitionUpdateTime = std::string(ptrVal, SysStringLen(ptrVal));
        SysFreeString(ptrVal);
    } else {
        throw std::runtime_error("Failed to query AV product definition state.");
    }
}
