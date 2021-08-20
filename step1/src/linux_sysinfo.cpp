#include <sys/utsname.h>
#include <iostream>

struct system_info : utsname {
  system_info() { uname(this); }
};

int main() {
  system_info oSysInfo;
  std::cout << "System Info\nName: " << oSysInfo.sysname
    << "\nRelease: " << oSysInfo.release
    << "\nVersion: " << oSysInfo.version
    << "\nMachine: " << oSysInfo.machine << std::endl;
  return 0;
}
