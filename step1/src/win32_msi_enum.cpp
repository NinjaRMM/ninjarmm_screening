#include <tchar.h>
#include <windows.h>
#include <Msi.h>
#include <iostream>
#include <string>
#include <stdexcept>

using tstring = std::basic_string<TCHAR>;

/*
wraps a windows error into an exception
*/
struct win32_exception : std::exception {
  /*
  throws an exception based on the GetLastError when the condition is met
  */
  template <typename _ty, typename _cond> 
  static _ty throw_if(_ty test, _cond condition) {
    if (condition(test)) throw win32_exception();
    return test;
  }
  /*
  throws an exception based on the condition
  */
  template <typename _ty, typename _cond> static _ty throw_errnum(_ty test, _cond condition) {
    if (condition(test)) throw win32_exception(test);
    return test;
  }
  virtual ~win32_exception() = default;

  win32_exception(DWORD dwError) {
    const char* sTemp = nullptr;
    ::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&sTemp), 0, nullptr);
    _what += sTemp;
    ::LocalFree((HLOCAL)sTemp);
  }

  win32_exception() : win32_exception(::GetLastError()){}

  virtual const char* what() const override { return _what.c_str(); }
private:
  std::string _what;
};


int main() {
  try {
    for (DWORD i = 0;; ++i) {
      //get installed product GUID
      tstring sProductID(38, 0);
      auto iRet = win32_exception::throw_errnum(
        ::MsiEnumProducts(i, &sProductID[0]), 
        [](DWORD d) { return (ERROR_SUCCESS != d && ERROR_NO_MORE_ITEMS != d); });

      if (ERROR_NO_MORE_ITEMS == iRet) break;

      //get the size of the name buffer and allocate accordingly
      DWORD iPropSize = 0;
      iRet = win32_exception::throw_errnum(
        ::MsiGetProductInfo(sProductID.c_str(), INSTALLPROPERTY_INSTALLEDPRODUCTNAME, nullptr, &iPropSize),
        [](DWORD d) { return (ERROR_SUCCESS != d && ERROR_MORE_DATA != d && ERROR_UNKNOWN_PROPERTY != d); }
      );

      if (ERROR_UNKNOWN_PROPERTY == iRet) {
        std::cout << " - no name - " << std::endl;
        continue;
      }
      tstring sName(++iPropSize, 0);
      //fill the name buffer and print
      iRet = win32_exception::throw_errnum(
        ::MsiGetProductInfo(sProductID.c_str(), INSTALLPROPERTY_INSTALLEDPRODUCTNAME, &sName[0], &iPropSize),
        [](DWORD d) { return (ERROR_SUCCESS != d && ERROR_MORE_DATA != d && ERROR_UNKNOWN_PROPERTY != d); }
      );
      std::cout << sName << std::endl;
    }
    return 0;
  }
  catch (const win32_exception& ex) {
    std::cerr << "A WIN32 exception occurred: " << ex.what();
  }
  catch (...) {
    std::cerr << "An unexpected exception occurred.";
  }
  return 1;
}