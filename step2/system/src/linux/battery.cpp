#include "hwinfo/platform.h"

#ifdef HWINFO_UNIX

#include <fstream>
#include <filesystem>

#include "hwinfo/battery.h"

namespace hwinfo {

static std::string base_path = "/sys/class/power_supply/";

// =====================================================================================================================
std::string Battery::getVendor() const {
  if (_id < 0) {
    return "<unknown>";
  }
  std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "manufacturer");
  std::string vendor;
  if (vendor_file.is_open()) {
    getline(vendor_file, vendor);
    return vendor;
  }
  return "<unknown>";
}

std::string Battery::getModel() const {
  if (_id < 0) {
    return "<unknown>";
  }
  std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "model_name");
  std::string value;
  if (vendor_file.is_open()) {
    getline(vendor_file, value);
    return value;
  }
  return "<unknown>";
}

std::string Battery::getSerialNumber() const {
  if (_id < 0) {
    return "<unknown>";
  }
  std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "serial_number");
  std::string value;
  if (vendor_file.is_open()) {
    getline(vendor_file, value);
    return value;
  }
  return "<unknown>";
}

std::string Battery::getTechnology() const {
  if (_id < 0) {
    return "<unknown>";
  }
  std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "technology");
  std::string value;
  if (vendor_file.is_open()) {
    getline(vendor_file, value);
    return value;
  }
  return "<unknown>";
}

uint32_t Battery::getEnergyFull() const {
  if (_id < 0) {
    return 0;
  }
  std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "energy_full");
  std::string value;
  if (vendor_file.is_open()) {
    getline(vendor_file, value);
    return std::stoi(value);
  }
  return 0;
}

uint32_t Battery::energyNow() const {
  if (_id < 0) {
    return 0;
  }
  std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "energy_now");
  std::string value;
  if (vendor_file.is_open()) {
    getline(vendor_file, value);
    return std::stoi(value);
  }
  return 0;
}

bool Battery::charging() const {
  if (_id < 0) {
    return false;
  }
  std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "status");
  std::string value;
  if (vendor_file.is_open()) {
    getline(vendor_file, value);
    return value == "Charging";
  }
  return false;
}

bool Battery::discharging() const {
  return !charging();
}

// =====================================================================================================================
std::vector<Battery> getAllBatteries() {
  std::vector<Battery> batteries;
  int8_t id = 0;
  while (std::filesystem::exists(std::filesystem::path(base_path + "BAT" + std::to_string(id)))) {
    batteries.emplace_back(id++);
  }
  return batteries;
}

}  // namespace hwinfo

#endif