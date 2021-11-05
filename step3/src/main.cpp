#include <algorithm>
#include <iostream>

#include <objbase.h>

#include "CodeReview.h"

int main(void)
{
    auto hr = CoInitialize(0);
    if (FAILED(hr))
    {
        std::cout << "Failed to initialize COM library." << std::endl;
        return 1;
    }

    auto thirdPartyAVSoftwareMap = std::map<std::wstring, ThirdPartyAVSoftware>{};
    if (queryWindowsForAVSoftwareDataWSC(thirdPartyAVSoftwareMap))
    {
        std::for_each(thirdPartyAVSoftwareMap.begin(), thirdPartyAVSoftwareMap.end(),
            [](const std::pair<std::wstring, ThirdPartyAVSoftware>& softwarePair)
                {
                    const auto software = softwarePair.second;
                    std::wcout << software.Name << std::endl;
                    std::wcout << software.Description << std::endl;
                    std::wcout << software.DefinitionUpdateTime << std::endl;
                    std::cout << software.DefinitionStatus << std::endl;
                    //std::wcout << software.Version << std::endl;
                    std::wcout << software.ProductState << std::endl;
                    std::cout << "----" << std::endl;
                });
    }

    CoUninitialize();

    return 0;
}
