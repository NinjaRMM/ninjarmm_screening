#if defined(__APPLE__) || defined(linux)

#include "CmdExecuter.hpp"


std::list<std::string> CmdExecuter::getPipeFromCmd(const std::string& cmd) {
    std::list<std::string> out;
    char line[256];
    FILE* fpipe = popen(cmd.c_str(), "r");

    if (fpipe == nullptr) {
        return std::list<std::string>();
    }

    while (fgets(line, sizeof line, fpipe)) {
        auto lineStr = std::string{line};
        if ((lineStr.size() > 0) && (lineStr[lineStr.length() - 1] == '\n')) {
            lineStr.resize(lineStr.length() - 1);
        }
        out.emplace_back(lineStr);
    }
    pclose(fpipe);
    return out;
}

#endif
