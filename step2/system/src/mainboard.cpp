#include <fstream>

#include "hwinfo/WMIwrapper.h"

#include "hwinfo/mainboard.h"


namespace hwinfo {

MainBoard::MainBoard(const std::string &vendor,
                     const std::string &product,
                     const std::string &version,
                     const std::string &serialNumber)
  : _vendor(vendor), _name(product), _version(version), _serialNumber(serialNumber) {
}

std::string& MainBoard::vendor() {
  if (_vendor.empty()) {
    _vendor = getVendor();
  }
  return _vendor;
}

std::string& MainBoard::name() {
  if (_name.empty()) {
    _name = getName();
  }
  return _name;
}

std::string& MainBoard::version() {
  if (_version.empty()) {
    _version = getVersion();
  }
  return _version;
}

std::string& MainBoard::serialNumber() {
  if (_serialNumber.empty()) {
    _serialNumber = getSerialNumber();
  }
  return _serialNumber;
}

}  // namespace hwinfo