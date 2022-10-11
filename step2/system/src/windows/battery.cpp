#include "hwinfo/platform.h"

#ifdef HWINFO_WINDOWS

#include "hwinfo/battery.h"
#include "hwinfo/WMIwrapper.h"

namespace hwinfo {

std::string Battery::getVendor() const {
  return "<unknwon>";
}

std::string Battery::getModel() const {
  return _model;
}

std::string Battery::getSerialNumber() const {
  return "<unknwon>";
}

std::string Battery::getTechnology() const {
  return "<unknwon>";
}

uint32_t Battery::getEnergyFull() const {
  return 0;
}

uint32_t Battery::energyNow() const {
  return 0;
}

bool Battery::charging() const {
  return false;
}

bool Battery::discharging() const {
  return false;
}

std::vector<Battery> getAllBatteries() {
  std::vector<Battery> batteries;
  std::vector<const wchar_t*> res {};
  wmi::queryWMI("Win32_Battery", "Name", res);
  if (res.empty() || res.at(0) == nullptr) { return {}; }
  int8_t counter = 0;
  for (const auto &v: res) {
    std::wstring tmp(v);
    batteries.emplace_back(counter++);
    batteries.back()._model = { tmp.begin(), tmp.end() };
  }
  res.clear();
  return batteries;
}

}  // namespace hwinfo

#endif