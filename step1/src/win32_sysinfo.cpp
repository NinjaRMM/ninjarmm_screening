/*
Windows example of retrieving some system info.

The key technique I intend to point out here is the C++ification of legacy C APIs via wrappers which is my prefered route when working with legacy C APIs.
Common to Windows is heavy use of C structures passed between the kernel and user space for various operations. I normally wrap these and
their associated functions into C++ style classes so the C <--> C++ translation is handled in a central location appropriate for the operation. In this example
the SYSTEM_INFO C structure is wrapped in a C++ class. The constructor populates the structure and a destructor would free any resources as needed. Member functions
would be exposed as needed which operate on the structure to wrap the legacy API for a more friendly C++ style. This enforces strict type safety and encapsulates any
neuances with the legacy API so the day-to-day work doesn't need to be concerned about internal details.

*/

#include <tchar.h>
#include <Windows.h>
#include <VersionHelpers.h>
#include <iostream>
#include <map>
#include <string>

using tstring = std::basic_string<TCHAR>;


struct SystemInfo : SYSTEM_INFO {
  SystemInfo() {
    ::GetSystemInfo(this);
  }
  void print() const {
    std::cout << "Processor Count: " << dwNumberOfProcessors << '\n';
    std::cout << "Processor architecture: ";
    switch (wProcessorArchitecture) {
      case PROCESSOR_ARCHITECTURE_INTEL:
        std::cout << "Intel\n"; break;
      case PROCESSOR_ARCHITECTURE_MIPS:
        std::cout << "MIPS\n"; break;
      case PROCESSOR_ARCHITECTURE_ALPHA:
        std::cout << "Alpha\n"; break;
      case PROCESSOR_ARCHITECTURE_PPC:
        std::cout << "PowerPC\n"; break;
      case PROCESSOR_ARCHITECTURE_SHX:
        std::cout << "SHX\n"; break;
      case PROCESSOR_ARCHITECTURE_ARM:
        std::cout << "ARM\n"; break;
      case PROCESSOR_ARCHITECTURE_IA64:
        std::cout << "Intel IA64\n"; break;
      case PROCESSOR_ARCHITECTURE_ALPHA64:
        std::cout << "Alpha 64\n"; break;
      case PROCESSOR_ARCHITECTURE_MSIL:
        std::cout << "MSIL Psudo Processor\n"; break;
      case PROCESSOR_ARCHITECTURE_AMD64:
        std::cout << "AMD x86-64\n"; break;
      case PROCESSOR_ARCHITECTURE_IA32_ON_WIN64:
        std::cout << "Intel IA32\n"; break;
      case PROCESSOR_ARCHITECTURE_ARM64:
      case PROCESSOR_ARCHITECTURE_ARM32_ON_WIN64:
      case PROCESSOR_ARCHITECTURE_IA32_ON_ARM64:
        std::cout << "ARM\n"; break;
      default:
        std::cout << "Uknown\n"; break;
    };
  }

};




int main() {
  SystemInfo oSysInfo;
  oSysInfo.print();
  std::cout << "Windows version: ";
  if (IsWindows10OrGreater()){
    std::cout << "10 or greater\n";
  }
  else if (IsWindows8Point1OrGreater()) {
    std::cout << "8.1\n";
  }
  else if (IsWindows8OrGreater()) {
    std::cout << "8\n";
  }
  else if (IsWindows7SP1OrGreater()) {
    std::cout << "7 SP1\n";
  }
  else if (IsWindows7OrGreater()) {
    std::cout << "7\n";
  }
  else if (IsWindowsVistaSP2OrGreater()) {
    std::cout << "Vista SP2\n";
  }
  else if (IsWindowsVistaSP1OrGreater()) {
    std::cout << "Vista SP1\n";
  }
  else if (IsWindowsXPSP3OrGreater()) {
    std::cout << "XP SP3\n";
  }
  else if (IsWindowsXPSP2OrGreater()) {
    std::cout << "XP SP2\n";
  }
  else if (IsWindowsXPSP1OrGreater()) {
    std::cout << "XP SP1\n";
  }
  else if (IsWindowsXPOrGreater()) {
    std::cout << "XP\n";
  }
  return 0;
}