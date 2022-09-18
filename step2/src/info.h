#pragma once

//--------------------------------------
// This is the public header
//--------------------------------------

#include <vector>
#include <string>
#include <tuple>

//--------------------------------------
using pairStrStr = std::tuple<std::string, std::string>;
using vecStr     = std::vector<std::string>;
using vecStrStr  = std::vector<pairStrStr>;

//--------------------------------------
bool QueryOsPatches(vecStr &strings);
bool QuerySystemInfo(vecStrStr &pairs);
