#include <sstream>

#include "pilot.h"

const std::string Pilot::m_defaultName("Pilot");
const std::string Pilot::m_defaultDescription("Driving a plane from takeoff to landing.");
const std::uint16_t Pilot::m_defaultHoursRequired(12);

Pilot::Pilot()
{
    m_name = m_defaultName;
    m_description = m_defaultDescription;
    m_hoursRequired = m_defaultHoursRequired;
}

Pilot::Pilot(const std::string& name, const std::string& description, const uint16_t& hoursRequired) : CommonJob(name, description, hoursRequired)
{
}

std::string Pilot::toString() const
{
    std::stringstream ss;
    ss << "name: " << getName() << " description: " << getDescription() << " hoursRequired: " << getHoursRequired();
    return ss.str();
}
