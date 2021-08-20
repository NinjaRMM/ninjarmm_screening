//requires libapt-pkg-dev

#include <iostream>
#include <apt-pkg/cachefile.h>
#include <apt-pkg/pkgcache.h>

int main() {
  try {
    pkgInitConfig(*_config);
    pkgInitSystem(*_config, _system);

    pkgCacheFile oCacheFile;
    auto oPkgCache = oCacheFile.GetPkgCache();

    for (auto oPackage = oPkgCache->PkgBegin() ; !oPackage.end() ; ++oPackage) {
      std::cout << oPackage.Name() << std::endl;
    }

    return 0;
  }
  catch (const std::exception& ex) {
    std::cerr << "An exception occurred: " << ex.what() << std::endl;
  }
  return 1;
}
