#include "hwinfo/platform.h"

#ifdef HWINFO_WINDOWS

#include <string>
#include <vector>

#include <Windows.h>

#include "hwinfo/WMIwrapper.h"
#include "hwinfo/ram.h"

namespace hwinfo {

std::string RAM::getVendor() {
  std::vector<const wchar_t*> names{};
  wmi::queryWMI("WIN32_PhysicalMemory", "Manufacturer", names);
  auto ret = names[0];
  if (!ret) {
    return "<unknown>";
  }
  std::wstring tmp(ret);
  return {tmp.begin(), tmp.end()};
}

std::string RAM::getName() {
  std::vector<const wchar_t*> names{};
  wmi::queryWMI("WIN32_PhysicalMemory", "Name", names);
  auto ret = names[0];
  if (!ret) {
    return "<unknown>";
  }
  std::wstring tmp(ret);
  return {tmp.begin(), tmp.end()};
}

std::string RAM::getModel() {
  std::vector<const wchar_t*> names{};
  wmi::queryWMI("WIN32_PhysicalMemory", "PartNumber", names);
  auto ret = names[0];
  if (!ret) {
    return "<unknown>";
  }
  std::wstring tmp(ret);
  return {tmp.begin(), tmp.end()};
}

std::string RAM::getSerialNumber() {
  std::vector<const wchar_t*> names{};
  wmi::queryWMI("WIN32_PhysicalMemory", "SerialNumber", names);
  auto ret = names[0];
  if (!ret) {
    return "<unknown>";
  }
  std::wstring tmp(ret);
  return {tmp.begin(), tmp.end()};
}

int64_t RAM::getTotalSize_Bytes() {
  MEMORYSTATUSEX status;
  status.dwLength = sizeof(status);
  GlobalMemoryStatusEx(&status);
  return static_cast<int64_t>(status.ullTotalPhys);
}

}  // namespace hwinfo

#endif  // HWINFO_WINDOWS