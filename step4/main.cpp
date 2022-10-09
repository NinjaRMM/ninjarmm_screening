#include "CodeReview.hpp"
#include "ThirdPartyAVSoftware.hpp"

#include <iostream>

void testQueryWindowsForAVSoftwareDataWSC() {
    using AntiVirusInfoSeeker::queryWindowsForAVSoftwareDataWSC;
    using AntiVirusInfoSeeker::ThirdPartyAVSoftwareMap;

    ThirdPartyAVSoftwareMap antiVirusMap;
    if (queryWindowsForAVSoftwareDataWSC(antiVirusMap)) {
        for (auto const& [name, antiVirus] : antiVirusMap) {
            std::wcout << L"Name: " << name << std::endl;
            std::wcout << L"Description: " << antiVirus.description << std::endl;
            std::wcout << L"DefinitionUpdateTime: " << antiVirus.definitionUpdateTime << std::endl;
            std::cout << "DefinitionStatus: " << antiVirus.definitionStatus << std::endl;
            std::wcout << L"Version: " << antiVirus.version << std::endl;
            std::wcout << L"ProductState: " << antiVirus.productState << std::endl;
            std::cout << std::endl;
        }
    }
}

int main() {
    testQueryWindowsForAVSoftwareDataWSC();
    return 0;
}
