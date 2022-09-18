#include "info_common.h"
//--
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>
//--
#include <sys/stat.h>
//--
#include "info.h"

#if defined(_MSC_VER)
    #define stat    _stat
    #define popen   _popen
    #define pclose  _pclose
#endif


//--------------------------------------
using unique_pipe = std::unique_ptr<std::FILE, decltype(&pclose)>;
using unique_file = std::unique_ptr<std::FILE, decltype(&fclose)>;

//--------------------------------------
bool
GetCommandLineOutput(const std::string &commandline, vecStr &strings) {
    unique_pipe pipe(popen(commandline.c_str(), "r"), pclose);
    if (!pipe) {
        return false;
    }

    char    buffer[1024];
    while (!feof(pipe.get())) {
        if (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
            strings.emplace_back(buffer);
        }
    }

    return true;
}

//--------------------------------------
bool
ParseKeyValueFile(const std::string &filename, mapStrStr &pairs) {
    unique_file file(fopen(filename.c_str(), "r"), &fclose);
    if (file == nullptr) {
        return false;
    }

    char    buffer[1024];
    while (!feof(file.get())) {
        if (fgets(buffer, sizeof(buffer), file.get()) != nullptr) {
            std::string str = buffer;

            // Remove spaces
            Trim(str, " \t");

            // Avoid comments
            if (str[0] == '#')
                continue;

            auto parts = Split(str, '=');
            if (parts.size() == 2) {
                pairs[Trim(parts[0], "\"")] = Trim(parts[1], "\"\r\n");
            }
        }
    }

    return true;
}

//--------------------------------------
bool
HasCommand(const std::string &command) {
    vecStr      strings;
    std::string whereis = "whereis ";

#if defined(__linux__)
    whereis += "-b ";
#endif

    if (GetCommandLineOutput(whereis + command, strings)) {
        if (strings.size() >= 1) {
            vecStr parts = Split(Trim(strings[0], "\" \t\n\r"), ':');
            if (parts.size() >= 2 && !parts[1].empty()) {
                return true;
            }
        }

#if defined(__APPLE__)
        return !strings.empty();
#endif
    }

    return false;
}

//--------------------------------------
bool
HasFile(const std::string& name) {
    struct stat buffer {};

    return (stat(name.c_str(), &buffer) == 0);
}

//--------------------------------------
void
ParseIfConfig(vecStrStr &pairs) {
    vecStr      strings;
    uint32_t    interface = 1, ip = 1;
    char        buffer[1024];

    if (GetCommandLineOutput("ifconfig -a", strings)) {
        for (const auto &line : strings) {
            if(line[0] != ' ' && line[0] != '\t') {
                auto pos = line.find(':');
                if(pos != std::string::npos) {
                    snprintf(buffer, sizeof(buffer), "Network Interface [%02u]", interface++);
                    pairs.emplace_back(buffer, line.substr(0, pos));
                    pos = line.find("UP");
                    if(pos != std::string::npos) {
                        pairs.emplace_back("                Status", "Media connected");
                    }
                    else {
                        pairs.emplace_back("                Status", "Media disconnected");
                    }
                    ip = 1;
                }
            }
            else {
                auto pos = line.find("inet");
                if(pos != std::string::npos) {
                    pos += 5;
                    if(line[pos-1] == '6') ++pos;
                    auto end  = line.find(' ', pos);
                    auto end2 = line.find('%', pos);    // On MacOS
                    if (end2 < end) end = end2;
                    snprintf(buffer, sizeof(buffer), "               IP [%02u]", ip++);
                    pairs.emplace_back(buffer, line.substr(pos, end-pos));
                }
            }
        }
    }
}

//--------------------------------------
void
ParseIp(vecStrStr &pairs) {
    vecStr strings;
    uint32_t  interface = 1, ip = 1;
    char      buffer[1024];

    if (GetCommandLineOutput("ip a", strings)) {
        for (const auto &line : strings) {
            if(line[0] != ' ') {
                auto pos = line.find(':');
                if(pos != std::string::npos) {
                    snprintf(buffer, sizeof(buffer), "Network Interface [%02u]", interface++);
                    auto pos2 = line.find(':', pos + 1);
                    pairs.emplace_back(buffer, line.substr(pos+2, pos2 - pos - 2));
                    pos = line.find("UP");
                    if(pos != std::string::npos) {
                        pairs.emplace_back("                Status", "Media connected");
                    }
                    else {
                        pairs.emplace_back("                Status", "Media disconnected");
                    }
                    ip = 1;
                }
            }
            else {
                auto pos = line.find("inet");
                if(pos != std::string::npos) {
                    pos += 5;
                    if(line[pos-1] == '6') ++pos;
                    auto end = line.find(' ', pos);
                    snprintf(buffer, sizeof(buffer), "               IP [%02u]", ip++);
                    pairs.emplace_back(buffer, line.substr(pos, end-pos));
                }
            }
        }
    }
}

//-------------------------------------
vecStr
Split(const std::string &str, char separator) {
    vecStr  output;
    size_t  prevPos = 0, pos = 0;

    while((pos = str.find(separator, pos)) != std::string::npos) {
        output.emplace_back(str.substr(prevPos, pos - prevPos));
        prevPos = ++pos;
    }
    // Last word
    output.emplace_back(str.substr(prevPos, pos - prevPos));

    return output;
}

//-------------------------------------
vecStr
Split(const std::string &str, const std::string &separator) {
    vecStr  output;
    size_t  prevPos = 0, pos = 0;

    while ((pos = str.find(separator, pos)) != std::string::npos) {
        output.emplace_back(str.substr(prevPos, pos - prevPos));
        prevPos = ++pos;
    }
    // Last word
    output.emplace_back(str.substr(prevPos, pos - prevPos));

    return output;
}

//-------------------------------------
std::string &
Trim(std::string &str, const char  *pChars) {
    ptrdiff_t   len;
    ptrdiff_t   start;
    ptrdiff_t   end;

    if(pChars == nullptr)
        return str;

    len     = ptrdiff_t(strlen(pChars));
    start   = 0;
    end     = ptrdiff_t(str.length() - 1);

    if(end >= start) {
        ptrdiff_t   c;
        bool        charFound;

        // Trim start
        do {
            charFound = false;
            for(c=0; c<len; c++) {
                if(str[start] == pChars[c]) {
                    charFound = true;
                    start++;
                    break;
                }
            }
        } while(charFound && start <= end);

        // Trim end
        if(end > start) {
            do
            {
                charFound = false;
                for(c=0; c<len; c++) {
                    if(str[end] == pChars[c]) {
                        charFound = true;
                        end--;
                        break;
                    }
                }
            } while(charFound && start <= end);
        }

        // Do it!
        if(start <= end)
            str = str.substr(start, end - start + 1);
        else
            str.clear();
    }

    return str;
}

//--------------------------------------
std::string &
RemoveExtraSpaces(std::string &str) {
    std::string aux;
    bool        found = false;

    for(char c : str) {
        if(found && c == ' ') {
            continue;
        }
        found = false;
        if(c == ' ') {
            found = true;
        }
        aux += c;
    }
    str = aux;

    return str;
}

//--------------------------------------
static const uint8_t  UTF8_Bytes[256] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,

    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,

    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,

    4,4,4,4,4,4,4,4,5,5,5,5,6,6,6,6,
};

//--------------------------------------
std::string &
RemoveNonASCII(std::string &str) {
    std::string aux;

    aux.reserve(str.size());
    for (size_t i = 0; i < str.size(); ++i) {
        uint8_t c = str[i];
        if (UTF8_Bytes[c] > 1) {
            i += UTF8_Bytes[c] - 1;
            c = ' ';
        }
        aux += c;
    }
    str = aux;

    return str;
}
