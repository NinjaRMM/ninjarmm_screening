//**************************************************************
// Sam Thomsen
// NinjaOne Code Screening
// January 2023
//**************************************************************

#include "CodeReview.h"

//**************************************************************
// int main()
//**************************************************************

int main()
{
    // get map
    std::map<std::wstring, ThirdPartyAVSoftware> products;

    // start com
    CoInitialize( NULL );

    // load products
    queryWindowsForAVSoftwareDataWSC( products );

    // stop com
    CoUninitialize();

    // for each result...
    for ( const auto &p : products )
    {
        // stream out name
        std::wcout << p.second.Name << std::endl;
    }

    // return
    return 0;
}
