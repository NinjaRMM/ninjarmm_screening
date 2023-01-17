//**************************************************************
// Sam Thomsen
// NinjaOne Code Screening
// January 2023
//**************************************************************

#ifndef __INFO_WIN_H__
#define __INFO_WIN_H__

#include "info-base.h"

#include <Windows.h>
#include <vector>

//**************************************************************
// class InfoWin
//**************************************************************

class InfoWin : public InfoBase
{
public:

    // constructor
    InfoWin();

    // system id
    std::wstring systemID() override;

    // platform
    std::wstring platform() override;

    // installed ram
    int64_t installedRAM() override;

    // available ram
    int64_t availableRAM() override;

    // cpu count
    int cpuCount() override;

    // cpu details
    std::wstring cpuInfo() override;

    // update count
    int updateCount() override;

    // update info
    std::wstring updateInfo( int i ) override;

private:

    // load basic info
    void loadBasicInfo();

    // load memory info
    void loadMemoryInfo();

    // load update info
    void loadUpdateInfo();

    // id
    std::wstring _systemID;

    // installed ram
    int64_t _installedRAM = 0;

    // available ram
    int64_t _availableRAM = 0;

    // cpu count
    int _cpuCount = 0;

    // cpu info
    std::wstring _cpuInfo;

    // update info
    std::vector<std::wstring> _updateInfo;
};

#endif
