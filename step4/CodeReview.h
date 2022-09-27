#pragma once

#include <string>
#include <map>
#include <windows.h>
#include <iwscapi.h>
#include <Wscapi.h>
#include <iostream>
#pragma comment(lib, "Wscapi.lib")
struct ThirdPartyAVSoftware;

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap);

