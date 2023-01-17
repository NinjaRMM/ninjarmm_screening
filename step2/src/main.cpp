//**************************************************************
// Sam Thomsen
// NinjaOne Code Screening
// January 2023
//
// Notes:
//
// In some cases, the solution relies on parsing files and
// scraping results from system commands, which is not ideal as
// the results may not be stable over OS revisions. More
// research should be done to ensure the querying techniques are
// indeed authoritative and will remain stable over time.
//
// The info implementation classes stand on their own and should
// be incorporated into unit tests that would run on systems
// with known resources for validation.
//
// All three implementations should build with the provided
// makefiles. Windows requires MS VC++ (cl) and both Linux and
// macOS require g++.
//
//**************************************************************

#include "info-base.h"

#include <cinttypes>
#include <iostream>

//**************************************************************
// int main()
//**************************************************************

int main()
{
    // result
    int result = 0;

    // try...
    try
    {
        // create info engine
        auto info = std::unique_ptr<InfoBase>( InfoBase::create() );

        // hostname
        std::wcout << "Hostname: " << info->systemID() << "\n";

        // platform
        std::wcout << "Platform: " << info->platform() << "\n";

        // ram
        std::wcout << "RAM: Installed: " << info->installedRAM() << "MB, Available: " << info->availableRAM() << "MB\n";

        // cpu
        std::wcout << "CPU: Count: " << info->cpuCount() << ", Description: " << info->cpuInfo() << "\n";

        // updates
        std::wcout << "Installed Updates:\n\n";

        // for each update...
        for ( int n = 0; n < info->updateCount(); ++n )
        {
            // print update
            std::wcout << "\t" << info->updateInfo( n ) << "\n";
        }
    }

    // catch...
    catch ( const std::exception &x )
    {
        // error
        std::cerr << "Error: " << x.what() << "\n\n";

        // error
        result = 1;
    }

    // return
    return result;
}
