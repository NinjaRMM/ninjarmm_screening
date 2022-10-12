#include <string>
#include <vector>

#if defined(unix) || defined(__unix) || defined(__unix__)
#include <unistd.h>
#elif defined(__APPLE__)
#include <sys/sysctl.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include "hwinfo/WMIwrapper.h"
#endif
#include "hwinfo/ram.h"

namespace hwinfo {

RAM::RAM(std::string &vendor,
         std::string &name,
         std::string &model,
         std::string &serialNumber,
         int64_t size_Bytes)
  : _vendor(vendor), _name(name), _model(model), _serialNumber(serialNumber) {
  _totalSize_Bytes = size_Bytes;
}

std::string &RAM::vendor() {
  if (_vendor.empty()) {
    _vendor = getVendor();
  }
  return _vendor;
}

std::string &RAM::name() {
  if (_name.empty()) {
    _name = getName();
  }
  return _name;
}

std::string &RAM::model() {
  if (_model.empty()) {
    _model = getModel();
  }
  return _model;
}

std::string &RAM::serialNumber() {
  if (_serialNumber.empty()) {
    _serialNumber = getSerialNumber();
  }
  return _serialNumber;
}

int64_t RAM::totalSize_Bytes() {
  if (_totalSize_Bytes == -1) {
    _totalSize_Bytes = getTotalSize_Bytes();
  }
  return _totalSize_Bytes;
}

}  // namespace hwinfo