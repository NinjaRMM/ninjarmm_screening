#include "hwinfo/platform.h"

#ifdef HWINFO_UNIX

#include <string>
#include <vector>

#include <unistd.h>

#include "hwinfo/ram.h"

namespace hwinfo {

std::string RAM::getVendor() {
  return "<unknown>";
}

std::string RAM::getName() {
  return "<unknown>";
}

std::string RAM::getModel() {
  return "<unknown>";
}

std::string RAM::getSerialNumber() {
  return "<unknown>";
}

int64_t RAM::getTotalSize_Bytes() {
  long pages = sysconf(_SC_PHYS_PAGES);
  long page_size = sysconf(_SC_PAGESIZE);
  if ((pages > 0) && (page_size > 0)) {
    return pages * page_size;
  }
  return -1;
}

}  // namespace hwinfo

#endif  // HWINFO_UNIX