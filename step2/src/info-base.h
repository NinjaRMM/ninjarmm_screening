//**************************************************************
// Sam Thomsen
// NinjaOne Code Screening
// January 2023
//**************************************************************

#ifndef __INFO_BASE_H__
#define __INFO_BASE_H__

#include <memory>
#include <string>

class InfoBase
{
public:

    // destructor
    virtual ~InfoBase() {};

    // system id
    virtual std::wstring systemID() = 0;

    // platform
    virtual std::wstring platform() = 0;

    // installed ram
    virtual int64_t installedRAM() = 0;

    // available ram
    virtual int64_t availableRAM() = 0;

    // cpu count
    virtual int cpuCount() = 0;

    // cpu details
    virtual std::wstring cpuInfo() = 0;

    // update count
    virtual int updateCount() = 0;

    // update info
    virtual std::wstring updateInfo( int i ) = 0;

    // static class factory
    static InfoBase *create() noexcept( false );
};

#endif
