#include "common_job.h"

CommonJob::CommonJob()
{
    m_name = "";
    m_description = "";
    m_hoursRequired = 0;
}

CommonJob::CommonJob(const std::string& name, const std::string& description, const uint16_t& hoursRequired)
    : m_name(name), m_description(description), m_hoursRequired(hoursRequired)
{
}

std::string CommonJob::getName() const
{
    return m_name;
}

std::string CommonJob::getDescription() const
{
    return m_description;
}

int CommonJob::getHoursRequired() const
{
    return m_hoursRequired;
}

void CommonJob::doWork() const
{
    std::cout << "My work involves " << getDescription() << std::endl;
}
