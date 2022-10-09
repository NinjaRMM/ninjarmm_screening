#pragma once

#include <list>
#include <string>


namespace CmdExecuter {
    std::list<std::string> getPipeFromCmd(const std::string& cmd);
};
