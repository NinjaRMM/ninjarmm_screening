#pragma once

//--------------------------------------
// This is a private header
//--------------------------------------

#include <unordered_map>
//--
#include "info.h"

//--------------------------------------
using mapStrStr = std::unordered_map<std::string, std::string>;

//--------------------------------------
bool            GetCommandLineOutput(const std::string &commandline, vecStr &strings);
bool            ParseKeyValueFile(const std::string &filename, mapStrStr &pairs);
bool            HasCommand(const std::string &command);
bool            HasFile(const std::string& name);
void            ParseIfConfig(vecStrStr &pairs);
void            ParseIp(vecStrStr &pairs);

vecStr          Split(const std::string &str, char separator);
vecStr          Split(const std::string &str, const std::string &separator);

std::string &   Trim(std::string &str, const char  *pChars);

std::string &   RemoveExtraSpaces(std::string &str);
std::string &   RemoveNonASCII(std::string &str);