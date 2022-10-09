#if defined(_WIN32)

#include "OSPatches.hpp"
#include "common/ScopedCoInitialize.hpp"
#include "sdkddkver.h"
#include <ShlObj.h>
#include <atlbase.h>
#include <sstream>
#include <codecvt>


std::list<std::string> OSPatches::get() const {
    auto scopedCoInitialize = ScopedCoInitialize();

    std::list<std::string> out;
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;

    CComPtr<IShellItem> pUpdates;
    CComPtr<IEnumShellItems> pShellEnum;

    auto hResult = SHGetKnownFolderItem(FOLDERID_AppUpdates, static_cast<KNOWN_FOLDER_FLAG>(0), nullptr, IID_PPV_ARGS(&pUpdates));
    if (FAILED(hResult)) {
        return {};
    }
    
    hResult = pUpdates->BindToHandler(nullptr, BHID_EnumItems, IID_PPV_ARGS(&pShellEnum));
    if (FAILED(hResult)) {
        return {};
    }

    if (!pShellEnum) {
        return {};
    }

    do {
        CComPtr<IShellItem> pItem;
        CComHeapPtr<WCHAR> szName;

        hResult = pShellEnum->Next(1, &pItem, nullptr);
        if (pItem) {
            pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &szName);
            out.emplace_back(conv.to_bytes(static_cast<LPWSTR>(szName)));
        }
    } while (hResult == S_OK);

    return out;
}

#endif
