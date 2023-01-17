//**************************************************************
// Sam Thomsen
// NinjaOne Code Screening
// January 2023
//**************************************************************

#include "info-win.h"

#include <algorithm>
#include <sstream>
#include <wuapi.h>

//**************************************************************
// InfoWin::InfoWin()
//**************************************************************

InfoWin::InfoWin()
{
    // load basic
    loadBasicInfo();

    // load mem
    loadMemoryInfo();

    // load update info
    loadUpdateInfo();
}

//**************************************************************
// std::wstring InfoWin::systemID()
//**************************************************************

std::wstring InfoWin::systemID()
{
    // return
    return _systemID;
}

//**************************************************************
// std::wstring InfoWin::platform()
//**************************************************************

std::wstring InfoWin::platform()
{
    // return
    return L"Windows";
}

//**************************************************************
// int64_t InfoWin::installedRAM()
//**************************************************************

int64_t InfoWin::installedRAM()
{
    // return
    return _installedRAM;
}

//**************************************************************
// int64_t InfoWin::availableRAM()
//**************************************************************

int64_t InfoWin::availableRAM()
{
    // return
    return _availableRAM;
}

//**************************************************************
// int InfoWin::cpuCount()
//**************************************************************

int InfoWin::cpuCount()
{
    // return
    return _cpuCount;
}

//**************************************************************
// std::wstring InfoWin::cpuInfo()
//**************************************************************

std::wstring InfoWin::cpuInfo()
{
    // return
    return _cpuInfo;
}

//**************************************************************
// int InfoWin::updateCount()
//**************************************************************

int InfoWin::updateCount()
{
    // return
    return int( _updateInfo.size() );
}

//**************************************************************
// std::wstring InfoWin::updateInfo()
//**************************************************************

std::wstring InfoWin::updateInfo( int i )
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
// void InfoWin::loadBasicInfo()
//**************************************************************

void InfoWin::loadBasicInfo()
{
    // string stream
    std::wstringstream wss;

    // name
    WCHAR name[ MAX_COMPUTERNAME_LENGTH + 1 ];

    // calc size
    DWORD size = sizeof( name ) / sizeof( WCHAR );

    // if success...
    if ( GetComputerNameW( name, &size ))
    {
        // covert name
        wss << name;

        // capture
        _systemID = wss.str();
    }

    // info
    SYSTEM_INFO sysInfo;

    // get info
    GetSystemInfo( &sysInfo );

    // cpu count
    _cpuCount = int( sysInfo.dwNumberOfProcessors );

    // reset stream
    wss.str( std::wstring() );

    // add details
    wss << "Type: "    << sysInfo.dwProcessorType           //.
        << ", Arch: "  << sysInfo.wProcessorArchitecture    //.
        << ", Rev: "   << sysInfo.wProcessorRevision        //.
        << ", Level: " << sysInfo.wProcessorLevel;          //.

    // capture it
    _cpuInfo = wss.str();
}

//**************************************************************
// void InfoWin::loadMemoryInfo()
//**************************************************************

void InfoWin::loadMemoryInfo()
{
    // memory info
    MEMORYSTATUSEX memoryInfo;

    // set struct size
    memoryInfo.dwLength = sizeof( memoryInfo );

    // if error...
    if ( !GlobalMemoryStatusEx( &memoryInfo ))
    {
        // throw
        throw std::exception( "Could not query memory information" );
    }

    // capture vals
    _installedRAM = memoryInfo.ullTotalPhys / 1000000;

    // calc available from memory load
    _availableRAM = ( memoryInfo.dwMemoryLoad * _installedRAM ) / 100;
}

//**************************************************************
// void InfoWin::loadUpdateInfo()
//**************************************************************

void InfoWin::loadUpdateInfo()
{
    // init com
    HRESULT hr = CoInitialize( NULL );

    // if error...
    if ( FAILED( hr ))
    {
        // throw
        throw std::exception( "Could not initialize COM" );
    }

    // init objects
    IUpdateSession    *pSession  = NULL;    //.
    IUpdateSearcher   *pSearcher = NULL;    //.
    ISearchResult     *pResult   = NULL;    //.
    IUpdateCollection *pUpdates  = NULL;    //.
    IUpdate           *pUpdate   = NULL;    //.

    // try...
    try
    {
        // create instance
        hr = CoCreateInstance( __uuidof( UpdateSession ), NULL, CLSCTX_INPROC_SERVER, __uuidof( IUpdateSession ), ( LPVOID * )&pSession );

        // if error...
        if ( FAILED( hr ))
        {
            // throw
            throw std::exception( "Could not create IUpdateSession" );
        }

        // create searcher
        hr = pSession->CreateUpdateSearcher( &pSearcher );

        // if error...
        if ( FAILED( hr ))
        {
            // throw
            throw std::exception( "Could not create UpdateSearcher" );
        }

        // allocate search string
        BSTR search = SysAllocString( L"IsInstalled=1" ); 

        // execute search
        hr = pSearcher->Search( search, &pResult );

        // free string
        SysFreeString( search );

        // if error...
        if ( FAILED( hr ))
        {
            // throw
            throw std::exception( "Failed to search for installed updates" );
        }

        // get updates
        hr = pResult->get_Updates( &pUpdates );

        // if error...
        if ( FAILED( hr ))
        {
            // throw
            throw std::exception( "Could not retreive installed updates" );
        }

        // count
        LONG count = 0;

        // get count
        pUpdates->get_Count( &count );

        // for each update...
        for ( LONG i = 0; i < count; ++i )
        {
            // reset update
            pUpdate = NULL;

            // get item
            hr = pUpdates->get_Item( i, &pUpdate );

            // if error...
            if ( FAILED( hr ))
            {
                // throw
                throw std::exception( "Problem geting update item" );
            }

            // title
            BSTR title;

            // get title
            pUpdate->get_Title( &title );

            // save update
            _updateInfo.push_back( title );

            // clear title
            SysFreeString( title );

            // clean up
            pUpdate->Release();
        }

        // clean up objects
        pUpdates ->Release();    //.
        pResult  ->Release();    //.
        pSearcher->Release();    //.
        pSession ->Release();    //.

        // un-init com
        CoUninitialize();
    }

    // catch...
    catch ( const std::exception &x )
    {
        // conditionally clean up objects
        if ( pUpdate   ) pUpdate  ->Release();    //.
        if ( pUpdates  ) pUpdates ->Release();    //.
        if ( pResult   ) pResult  ->Release();    //.
        if ( pSearcher ) pSearcher->Release();    //.
        if ( pSession  ) pSession ->Release();    //.

        // un-init com
        CoUninitialize();

        // throw exception copy
        throw x;
    }
}

//**************************************************************
// InfoBase *InfoBase::create()
//
// Static class factory for Windows implementation. Relies on a
// build regime that excludes other platforms from consideration
// during linking.
//**************************************************************

InfoBase *InfoBase::create()
{
    // return engine
    return new InfoWin();
}
