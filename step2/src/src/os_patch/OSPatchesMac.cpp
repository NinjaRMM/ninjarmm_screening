#if defined(__APPLE__)

#include "OSPatches.hpp"
#include "common/CmdExecuter.hpp"


std::list<std::string> OSPatches::get() const {
    //It may not be the most assertive option to get patches, but it is simpler.
    auto listPatches = CmdExecuter::getPipeFromCmd(R"(softwareupdate --history | grep -v "^Display Name\|^----")");
    return listPatches;
}

#endif
