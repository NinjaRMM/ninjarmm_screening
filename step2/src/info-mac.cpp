//**************************************************************
// Sam Thomsen
// NinjaOne Code Screening
// January 2023
//**************************************************************

#include "info-mac.h"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <mach/mach_host.h>
#include <mach/mach_init.h>
#include <mach/mach_types.h>
#include <mach/vm_statistics.h>
#include <set>
#include <sstream>
#include <sys/sysctl.h>
#include <sys/utsname.h>
#include <unistd.h>

//**************************************************************
// class exception
//
// Need an exception with a string member
//**************************************************************

class exception : public std::exception
{
public:

    // constructor
    exception( const char *what ):
        _what( what )
    {
    }

    // override what
    const char *what() const noexcept override
    {
        // return
        return _what.c_str();
    }

private:

    // message
    std::string _what;
};

//**************************************************************
// InfoMac::InfoMac()
//**************************************************************

InfoMac::InfoMac()
{
    // load basic
    loadBasicInfo();

    // load mem
    loadMemoryInfo();

    // load update info
    loadUpdateInfo();
}

//**************************************************************
// std::wstring InfoMac::systemID()
//**************************************************************

std::wstring InfoMac::systemID()
{
    // return
    return _systemID;
}

//**************************************************************
// std::wstring InfoMac::platform()
//**************************************************************

std::wstring InfoMac::platform()
{
    // return
    return L"macOS";
}

//**************************************************************
// int64_t InfoMac::installedRAM()
//**************************************************************

int64_t InfoMac::installedRAM()
{
    // return
    return _installedRAM;
}

//**************************************************************
// int64_t InfoMac::availableRAM()
//**************************************************************

int64_t InfoMac::availableRAM()
{
    // return
    return _availableRAM;
}

//**************************************************************
// int InfoMac::cpuCount()
//**************************************************************

int InfoMac::cpuCount()
{
    // return
    return _cpuCount;
}

//**************************************************************
// std::wstring InfoMac::cpuInfo()
//**************************************************************

std::wstring InfoMac::cpuInfo()
{
    // return
    return _cpuInfo;
}

//**************************************************************
// int InfoMac::updateCount()
//**************************************************************

int InfoMac::updateCount()
{
    // return
    return int( _updateInfo.size() );
}

//**************************************************************
// std::wstring InfoMac::updateInfo( int i )
//**************************************************************

std::wstring InfoMac::updateInfo( int i )
{
    // result
    std::wstring result;

    // if in range...
    if ( i < int( _updateInfo.size() ))
    {
        // capture it
        result = _updateInfo[ i ];
    }

    // return
    return result;
}

//**************************************************************
// void InfoMac::loadBasicInfo()
//**************************************************************

void InfoMac::loadBasicInfo()
{
    // string stream
    std::wstringstream wss;

    // max hostname
    std::size_t host_name_max = 64;

    // string
    char hostname[ host_name_max + 1 ];

    // get name
    gethostname( hostname, host_name_max );

    // convert string
    wss << hostname;

    // system id
    _systemID = wss.str();

    // cpu count
    _cpuCount = sysconf( _SC_NPROCESSORS_ONLN );

    // buffer size
    std::size_t cpu_info_max = 128;

    // cpu info buffer
    char cpuInfoBuffer[ cpu_info_max + 1 ];

    // if problem getting cpu info...
    if ( sysctlbyname( "machdep.cpu.brand_string", cpuInfoBuffer, &cpu_info_max, NULL, 0 ) < 0 )
    {
        // throw
        throw exception( "Couldn't query system for CPU info" );
    }

    // clear stream
    wss.str( std::wstring() );

    // stream val
    wss << cpuInfoBuffer;

    // capture it
    _cpuInfo = wss.str();
}

//**************************************************************
// void InfoMac::loadMemoryInfo()
//**************************************************************

void InfoMac::loadMemoryInfo()
{
    // vm info
    vm_size_t              page_size;
    vm_statistics64_data_t vm_stats;

    // get machine port
    mach_port_t mach_port = mach_host_self();

    // calc count
    mach_msg_type_number_t count = sizeof( vm_stats ) / sizeof( natural_t );

    // if can't query for memory info...
    if (( host_page_size( mach_port, &page_size ) != KERN_SUCCESS ) ||
        ( host_statistics64( mach_port, HOST_VM_INFO, ( host_info64_t )&vm_stats, &count ) != KERN_SUCCESS ))
    {
        // throw
        throw exception( "Couldn't query system for memory info" );
    }

    // calc available
    _availableRAM = ( int64_t )vm_stats.free_count * ( int64_t )page_size;

    // calc used
    int64_t usedRAM = (( int64_t )vm_stats.active_count + ( int64_t )vm_stats.inactive_count + ( int64_t )vm_stats.wire_count ) * ( int64_t )page_size;

    // calc total
    _installedRAM = usedRAM + _availableRAM;

    // convert to MB
    _availableRAM /= 1000000;
    _installedRAM /= 1000000;
}

//**************************************************************
// void InfoMac::loadUpdateInfo()
//
// Simple parser makes some assumptions about the XML format
// of the InstallHistory file. If assumptions do not hold, it
// won't crash, but will exit with error.
//**************************************************************

void InfoMac::loadUpdateInfo()
{
    // open file
    std::ifstream file( "/Library/Receipts/InstallHistory.plist" );

    // if not open...
    if ( !file.is_open() )
    {
        // throw
        throw exception( "Problem opening system install history" );
    }

    // states
    enum State
    {
        NEXT,            // looking for next
        READYNAME,       // looking for name tag
        FOUNDNAME,       // read name
        READYVERSION,    // looking for version tag
        FOUNDVERSION,    // read version
    };

    // init state
    State state = NEXT;

    // line
    std::string line;

    // update item
    std::string updateLine;

    // temp set for sorting and uniqueness
    std::set<std::string> tempList;

    // string tag
    std::string begin = "<string>";

    // while line...
    while ( std::getline( file, line ))
    {
        // if next...
        if ( line.find( "<dict>" ) != std::string::npos )
        {
            // switch on state
            switch ( state )
            {
            case NEXT: state = READYNAME; break;
            case READYNAME:
            case FOUNDNAME:
            case READYVERSION:
            case FOUNDVERSION: throw exception( "Unexpected token parsing update information" );
            }
        }

        // else if name...
        else if ( line.find( ">displayName<" ) != std::string::npos )
        {
            // switch on state
            switch ( state )
            {
            case READYNAME: state = FOUNDNAME; break;
            case NEXT:
            case FOUNDNAME:
            case READYVERSION:
            case FOUNDVERSION: throw exception( "Unexpected token parsing update information" );
            }
        }

        // else if name...
        else if ( line.find( ">displayVersion<" ) != std::string::npos )
        {
            // switch on state
            switch ( state )
            {
            case READYVERSION: state = FOUNDVERSION; break;
            case NEXT:
            case READYNAME:
            case FOUNDNAME:
            case FOUNDVERSION: throw exception( "Unexpected token parsing update information" );
            }
        }

        // else if found name...
        else if ( state == State::FOUNDNAME )
        {
            // find string
            auto start = line.find( begin ) + begin.size();

            // end is next
            auto end = line.find( "<", start );

            // capture line
            updateLine = line.substr( start, end - start );

            // update state
            state = READYVERSION;
        }

        // else if found version
        else if ( state == State::FOUNDVERSION )
        {
            // find string
            auto start = line.find( begin ) + begin.size();

            // end is next
            auto end = line.find( "<", start );

            // capture line
            updateLine += " " + line.substr( start, end - start );

            // add to set
            tempList.insert( updateLine );

            // update state
            state = NEXT;
        }
    }

    // for each temp update...
    for ( const auto &update : tempList )
    {
        // string stream
        std::wstringstream wss;

        // capture update
        wss << update.c_str();

        // add update to list
        _updateInfo.push_back( wss.str() );
    }

    // close file
    file.close();
}

//**************************************************************
// Static class factory for Mac implementation. Relies on a
// build regime that excludes other platforms from consideration
// during linking.
//**************************************************************

InfoBase *InfoBase::create()
{
    // return engine
    return new InfoMac();
}
