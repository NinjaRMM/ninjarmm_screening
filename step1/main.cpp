#include <iostream>
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
#define PIPE_OPEN _popen
#define PIPE_CLOSE _pclose
#else
#define PIPE_OPEN popen
#define PIPE_CLOSE pclose
#endif

// Note: for any bigger program, using boost.process, reproc or a custom library that actually
// manages processes with proper calls will be better.
std::string call_process(const std::string &commandline)
{
    char buffer[255] = {0};
    std::stringstream result;
    FILE *pipe = PIPE_OPEN(commandline.c_str(), "r");

    if (!pipe)
        throw std::runtime_error("popen() failed!");
    try
    {
        while (!feof(pipe))
        {
            if (fgets(buffer, sizeof(buffer), pipe) != NULL)
                result << buffer;
        }
    }
    catch (...)
    {
        PIPE_CLOSE(pipe);
        throw;
    }
    PIPE_CLOSE(pipe);
    return result.str();
}

void get_patches_and_packages()
{
#if defined(_WIN32) || defined(_WIN64)
    std::cout << call_process("wmic qfe list") << std::endl;
#elif defined(__MACH__) || defined(__APPLE__)
    std::cout << call_process("softwareupdate-- history-- all") << std::endl;
#elif defined(__linux__)
    // Actually, this is not enough, and should have preprocessor directives for
    // redhat/centos with yum, fedore with fnf, suse with zypper, arch with
    // pacman and more that im surely forgetting
    std::cout << call_process("dpkg -l") << std::endl;
#else
    std::cout << "Unsupported system" << std::endl;
#endif
}

void get_system_info()
{
#if defined(_WIN32) || defined(_WIN64)
    std::cout << call_process("systeminfo") << std::endl;
#elif defined(__MACH__) || defined(__APPLE__)
    std::cout << call_process("system_profiler") << std::endl;
#elif defined(__linux__)
    std::cout << call_process("uname -a") << std::endl;
    std::cout << call_process("hwinfo --short") << std::endl;
#else
    std::cout << "Unsupported system" << std::endl;
#endif
}

int main(int, char **)
{
    std::cout << "Packages/Patches: " << std::endl;
    get_patches_and_packages();
    std::cout << std::endl;
    std::cout << "System Information: " << std::endl;
    get_system_info();

}