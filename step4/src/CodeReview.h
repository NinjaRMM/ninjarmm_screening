#ifndef __CODEREVIEW_H__
#define __CODEREVIEW_H__

#include <iostream>
#include <iwscapi.h>
#include <map>
#include <string>
#include <Windows.h>
#include <wscapi.h>

//**************************************************************
// struct ThirdPartyAVSoftware
//**************************************************************

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string  DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

//**************************************************************
// bool queryWindowsForAVSoftwareDataWSC()
//**************************************************************

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap);

#endif
