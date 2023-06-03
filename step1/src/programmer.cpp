#include "programmer.h"

const std::string Programmer::m_defaultName("Programmer");
const std::string Programmer::m_defaultDescription("Designing, implementing and testing software projects.");
const std::uint16_t Programmer::m_defaultHoursRequired(8);

Programmer::Programmer()
{
    m_name = m_defaultName;
    m_description = m_defaultDescription;
    m_hoursRequired = m_defaultHoursRequired;
}

Programmer::Programmer(const std::string& name, const std::string& description, const uint16_t& hoursRequired) : CommonJob(name, description, hoursRequired)
{
}
