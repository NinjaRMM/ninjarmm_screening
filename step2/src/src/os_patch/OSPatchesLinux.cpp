#if defined(linux)

#include "OSPatches.hpp"
#include "common/CmdExecuter.hpp"


std::list<std::string> OSPatches::get() const {
    //As far as I know linux doesn't have operating system update patches. Only update packages (dpkg). So I put these dpkg packages.
    auto listPatches = CmdExecuter::getPipeFromCmd(R"(zcat /var/log/dpkg.log.*.gz | cat - /var/log/dpkg.log | grep 'upgrade' | cut -c29-)");
    return listPatches;
}

#endif
