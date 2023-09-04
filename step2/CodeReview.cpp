/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

#include "Win32APIEnumsAnsTypes.h"
#include "Mappers.h"
#include "WSCError.h"
#include "SoftwareDataWSCQuerier.h"

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyWSCSoftware> &thirdPartyWSCSoftwareMap)
{
    auto querier = SoftwareDataWSCQuerier{};
    auto builder = AntivirusSoftwareDataWSCQueryBuilder{thirdPartyWSCSoftwareMap};

    querier.QueryWindowsForSoftwareDataWSC(builder);

    return builder.AnythingBuilt();
}