#pragma once

#include <map>
#include <string>

//moved the struct to the header where it belongs
struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::wstring DefinitionStatus; // Adjusted to wstring, no obvious reason no to maintain the common string type.
    std::wstring Version; // TODO: populate this in queryWindowsForAVSoftwareDataWSC
    std::wstring ProductState;
};

// TODO: This needs some kind of API documentation here
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap);