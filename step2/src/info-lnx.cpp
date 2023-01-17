//**************************************************************
// Sam Thomsen
// NinjaOne Code Screening
// January 2023
//
// Note: 2 approaches were used to cover Centos and Ubuntu. More
// research should be performed to find the authoritative way
// to obtain system info for other Linux flavors.
//**************************************************************

#include "info-lnx.h"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <set>
#include <sstream>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <unistd.h>

//**************************************************************
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
// InfoLinux::InfoLinux()
//**************************************************************

InfoLinux::InfoLinux()
{
    // load basic
    loadBasicInfo();

    // load mem
    loadMemoryInfo();

    // load update info (dpkg)
    loadUpdateInfoDpkg();
}

//**************************************************************
// std::wstring InfoLinux::systemID()
//**************************************************************

std::wstring InfoLinux::systemID()
{
    // return
    return _systemID;
}

//**************************************************************
// std::wstring InfoLinux::platform()
//**************************************************************

std::wstring InfoLinux::platform()
{
    // return
    return L"Linux";
}

//**************************************************************
// int64_t InfoLinux::installedRAM()
//**************************************************************

int64_t InfoLinux::installedRAM()
{
    // return
    return _installedRAM;
}

//**************************************************************
// int64_t InfoLinux::availableRAM()
//**************************************************************

int64_t InfoLinux::availableRAM()
{
    // return
    return _availableRAM;
}

//**************************************************************
// int InfoLinux::cpuCount()
//**************************************************************

int InfoLinux::cpuCount()
{
    // return
    return _cpuCount;
}

//**************************************************************
// std::wstring InfoLinux::cpuInfo()
//**************************************************************

std::wstring InfoLinux::cpuInfo()
{
    // return
    return _cpuInfo;
}

//**************************************************************
// int InfoLinux::updateCount()
//**************************************************************

int InfoLinux::updateCount()
{
    // return
    return int( _updateInfo.size() );
}

//**************************************************************
// std::wstring InfoLinux::updateInfo( int i )
//**************************************************************

std::wstring InfoLinux::updateInfo( int i )
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
// void InfoLinux::loadBasicInfo()
//**************************************************************

void InfoLinux::loadBasicInfo()
{
    // string stream
    std::wstringstream wss;

    // string
    char hostname[ HOST_NAME_MAX + 1 ];

    // get name
    gethostname( hostname, HOST_NAME_MAX );

    // convert string
    wss << hostname;

    // system id
    _systemID = wss.str();

    // cpu count
    _cpuCount = sysconf( _SC_NPROCESSORS_ONLN );

    // line
    std::string line;

    // open file
    std::ifstream file( "/proc/cpuinfo" );

    // if not open...
    if ( !file.is_open() )
    {
        // throw
        throw exception( "Problem opening /proc/cpuinfo" );
    }

    // reset stream
    wss.str( std::wstring() );

    // init search
    std::string search = "model name";

    // while line...
    while ( std::getline( file, line ))
    {
        // find model name
        auto pos = line.find( search );

        // if model name...
        if ( pos != std::string::npos )
        {
            // find delimiter
            pos = line.find( ": " );

            // just in case
            pos = ( pos == std::string::npos ) ? search.size() : pos + 2;

            // capture it
            wss << line.substr( pos ).c_str();

            // stop looking
            break;
        }
    }

    // close file
    file.close();

    // capture cpu info
    _cpuInfo = wss.str();

    // if not found...
    if ( _cpuInfo.empty() )
    {
        // throw
        throw exception( "Could not find CPU info in /proc/cpuinfo" );
    }
}

//**************************************************************
// void InfoLinux::loadMemoryInfo()
//**************************************************************

void InfoLinux::loadMemoryInfo()
{
    // mem info
    struct sysinfo memInfo;

    // if error...
    if ( sysinfo( &memInfo ) == -1 )
    {
        // throw
        throw exception( "Problem calling sysinfo" );
    }

    // total memory
    _installedRAM = memInfo.totalram / 1000000;

    // availalbe memory
    _availableRAM = memInfo.freeram / 1000000;
}

//**************************************************************
// void InfoLinux::loadUpdateInfoDpkg()
//**************************************************************

void InfoLinux::loadUpdateInfoDpkg()
{
    // line
    std::string line;

    // open file
    std::ifstream file( "/var/log/dpkg.log" );

    // if not open...
    if ( !file.is_open() )
    {
        // try rpm version
        loadUpdateInfoRPM();
    }

    // else...
    else
    {
        // search string
        std::string search = "status installed ";

        // temp set for sorting and uniqueness
        std::set<std::string> tempList;

        // while line...
        while ( std::getline( file, line ))
        {
            // try to find...
            auto pos = line.find( search );

            // if installed...
            if ( pos != std::string::npos )
            {
                // add update temp list
                tempList.insert( line.substr( pos + search.size() ));
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
}

//**************************************************************
// void InfoLinux::loadUpdateInfoRPM()
//**************************************************************

void InfoLinux::loadUpdateInfoRPM()
{
    // run rpm
    int result = system( "rpm -qa --queryformat '%{NAME} %{VERSION}\\n' > /tmp/packages.txt" );

    // if bad...
    if ( result )
    {
        // throw
        throw exception( "Problem running rpm query" );
    }

    // open file
    std::ifstream file( "/tmp/packages.txt" );

    // if not open...
    if ( !file.is_open() )
    {
        // throw
        throw exception( "Problem opening rpm query results in /tmp/packages.txt" );
    }

    // line
    std::string line;

    // temp set for uniqueness
    std::set<std::string> tempList;

    // while line...
    while ( std::getline( file, line ))
    {
        // add update temp list
        tempList.insert( line );
    }

    // close file
    file.close();

    // delete file
    std::remove( "/tmp/packages.txt" );

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
}

//**************************************************************
// Static class factory for Linux implementation. Relies on a
// build regime that excludes other platforms from consideration
// during linking.
//**************************************************************

InfoBase *InfoBase::create()
{
    // return engine
    return new InfoLinux();
}
