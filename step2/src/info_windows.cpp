#if defined(_WIN32) || defined(_WIN64)

#include "info.h"
#include "info_common.h"

//--------------------------------------
bool
QueryOsPatches(vecStr &strings) {
    vecStr packages;

    // The output of winget seems to be UTF8 but I'm not sure, so I'm going to remove all non ASCII characters. 
    // I don't want to change the interface only for Windows and use wchar_t / wstring
    if (GetCommandLineOutput("where winget.exe", packages)) {
        if (!packages.empty() && packages[0].find("\\winget.exe") != std::string::npos) {
            if (GetCommandLineOutput("winget list", packages)) {
                if (packages.size() > 1) {
                    auto ini = packages[1].find("Name");
                    auto end = packages[1].find("Id");
                    if (ini != std::string::npos && end != std::string::npos) {
                        end -= ini;
                        for (size_t i = 3; i < packages.size(); ++i) {
                            auto str = packages[i].substr(0, end);
                            strings.emplace_back(RemoveNonASCII(str));
                        }
                    }
                }
            }
        }
    }

    GetCommandLineOutput("wmic qfe get hotfixid, InstalledOn, Description", strings);

    // Clean output
    for (auto &str : strings) {
        str = Trim(str, " \n\r");
    }

    return !strings.empty();
}

//--------------------------------------
static void
ParseProcessors(vecStrStr &pairs, const std::string &values) {
    vecStr processors = Split(values, ',');

    for (size_t j = 1; j < processors.size(); ++j) {
        vecStr parts = Split(processors[j], ':');
        if (parts.size() == 2) {
            pairs.emplace_back("Processor " + parts[0], Trim(parts[1], "\" \n\r"));
        }
    }
}

//--------------------------------------
static void
ParseHotfixes(vecStrStr &pairs, const std::string &values) {
    vecStr hotfixes = Split(values, ',');

    for (size_t j = 1; j < hotfixes.size(); ++j) {
        vecStr parts = Split(hotfixes[j], ':');
        if (parts.size() == 2) {
            pairs.emplace_back("Hotfix " + parts[0], Trim(parts[1], "\" \n\r"));
        }
    }
}

//--------------------------------------
static void
ParseNICs(vecStrStr &pairs, const std::string &values) {
    vecStr nics = Split(values, ',');
    bool findingIPs = false;

    for (size_t j = 1; j < nics.size(); ++j) {
        vecStr parts = Split(nics[j], ':');
        if (parts.size() == 1) {
            //pairs.emplace_back(Trim(parts[0], "\" \n\r"), "");
            if (parts[0].find("IP address") != std::string::npos) {
                findingIPs = true;
            }
        }
        else if (parts.size() == 2) {
            if (parts[0][0] == '[') {
                pairs.emplace_back("Network Interface " + parts[0], Trim(parts[1], "\" \n\r"));
                findingIPs = false;
            }
            else if (findingIPs && parts[0].find('[') != std::string::npos) {
                pairs.emplace_back("                        IP " + Trim(parts[0], " "), Trim(parts[1], "\" \n\r"));
            }
            else {
                pairs.emplace_back("                  " + parts[0], Trim(parts[1], "\" \n\r"));
                findingIPs = false;
            }
        }
        else {
            auto pos = nics[j].find("]:");
            if (pos != std::string::npos) {
                auto key = nics[j].substr(0, pos + 1);
                auto value = nics[j].substr(pos + 2);
                if (findingIPs) {
                    pairs.emplace_back("                        IP " + Trim(key, " "), Trim(value, "\" \n\r"));
                }
                else {
                    pairs.emplace_back("                  " + key, Trim(value, "\" \n\r"));
                    findingIPs = false;
                }
            }
        }
    }
}

//--------------------------------------
static void
ParseHyperV(vecStrStr &pairs, const vecStr &requirements) {
    for (const auto &requirement : requirements) {
        vecStr parts = Split(requirement, ':');
        if (parts.size() >= 2) {
            pairs.emplace_back("Hyper-V " + parts[0], Trim(parts[1], "\" \n\r"));
        }
    }
}

//--------------------------------------
bool
QuerySystemInfo(vecStrStr &pairs) {
    vecStr strings;

    if (GetCommandLineOutput("systeminfo /FO csv", strings)) {
        if (strings.size() > 2) {
            // There should be only 2 rows. If there are more, it is because the second file was too long.
            for (size_t i = 2; i < strings.size(); ++i) {
                strings[1] += strings[i];
            }
        }

        // Parse output
        if (strings.size() >= 2) {
            vecStr keys   = Split(strings[0], ",\"");
            vecStr values = Split(strings[1], ",\"");
            if (keys.size() >= values.size()) {
                for (size_t i = 0; i < keys.size(); ++i) {
                    if (keys[i].find("Processor(s)") != std::string::npos) {
                        ParseProcessors(pairs, values[i]);
                    }
                    else if (keys[i].find("Hotfix(s)") != std::string::npos) {
                        ParseHotfixes(pairs, values[i]);
                    }
                    else if (keys[i].find("Network Card(s)") != std::string::npos) {
                        ParseNICs(pairs, values[i]);
                    }
                    else if (keys[i].find("Hyper-V Requirements") != std::string::npos) {
                        vecStr requirements = Split(Trim(values[i], "\"\n\r "), ',');

                        if (requirements.size() > 1) {
                            ParseHyperV(pairs, requirements);
                        }
                        else {
                            pairs.emplace_back(Trim(keys[i], "\" \n\r"), Trim(values[i], "\" \n\r"));
                        }
                    }
                    else {
                        pairs.emplace_back(Trim(keys[i], "\" \n\r"), Trim(values[i], "\" \n\r"));
                    }
                }
                return true;
            }
        }
    }
    return false;
}

#endif
