#pragma once

#include <map>
#include <string>


struct ThirdPartyAVSoftware;

// Depending on the requirements, an interface class could be used here, 
// in order to have more flexibility of implementations and tests.
namespace AntiVirusInfoSeeker {
    // alias to improve rea readability and facilitate facilitate future changes
    using ThirdPartyAVSoftwareMap = std::map<std::wstring, ThirdPartyAVSoftware>;

    bool queryWindowsForAVSoftwareDataWSC(ThirdPartyAVSoftwareMap& thirdPartyAVSoftwareMap);
};
