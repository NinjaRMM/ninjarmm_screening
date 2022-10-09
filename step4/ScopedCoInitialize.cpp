#include "ScopedCoInitialize.hpp"

#include <windows.h>

ScopedCoInitialize::ScopedCoInitialize() {
    auto ciResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (ciResult == S_OK || ciResult == S_FALSE) {
        initialized_ = true;
    }
}

ScopedCoInitialize::~ScopedCoInitialize() {
    if (initialized_) {
        CoUninitialize();
    }
}
