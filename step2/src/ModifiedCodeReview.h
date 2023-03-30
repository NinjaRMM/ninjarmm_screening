#ifndef MODIFIED_CODE_REVIEW_H
#define MODIFIED_CODE_REVIEW_H

#include <map>
#include <string>

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    //REVIEW if description is not uniquely set maybe remove it from the structure?
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    //REVIEW is there a particular reasons this was set as string instead of wstring?
    std::wstring DefinitionStatus;
    //REVIEW if version is not set maybe remove it from the structure?
    std::wstring Version;
    std::wstring ProductState;
};


bool queryWindowsForAVSoftwareDataWSC_modified(
    std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap);

#endif //MODIFIED_CODE_REVIEW_H
