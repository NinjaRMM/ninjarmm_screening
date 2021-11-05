#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <algorithm>
#include <iostream>

#include <objbase.h>

#include "CodeReview.h"

bool Init();
void DisplayAVSoftware();
void Shutdown();

int main(void)
{
    if (!Init())
    {
        return 1;
    }

    DisplayAVSoftware();
    Shutdown();

    _CrtDumpMemoryLeaks();
    return 0;
}

bool Init()
{
    auto hr = CoInitialize(0);
    if (FAILED(hr))
    {
        std::cout << "Failed to initialize COM library." << std::endl;
        return false;
    }

    return true;
}

void DisplayAVSoftware()
{
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
}

void Shutdown()
{
    CoUninitialize();
}
