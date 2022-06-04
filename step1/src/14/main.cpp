#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>


bool getPipeOutput(FILE * const pipe, std::string& output)
{
    if (pipe == nullptr) { return false; }

    constexpr auto bufferSize = 32768UL;
    char buffer[bufferSize];

    output.reserve(bufferSize);

    while (feof(pipe) == 0)
    {
        if (fgets(buffer, bufferSize, pipe) == nullptr) 
        {
            continue;
        }

        output += buffer;
    }

    return true;
}

int main(int argc, char** argv)
{
    std::unique_ptr<FILE, std::function<void(FILE*)>> pipe;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

    auto pipeDeleter =
        [](FILE* pipe)
        {
            if (pipe == nullptr) { return; }

            _pclose(pipe);
            pipe = nullptr;
        };

    pipe  = std::unique_ptr<FILE, std::function<void(FILE*)>>(_popen("wmic qfe list", "r"), pipeDeleter);

#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_MAC

    auto pipeDeleter =
        [](FILE* pipe)
        {
            if (pipe == nullptr) { return; }

            pclose(pipe);
            pipe = nullptr;
        };

    pipe = std::unique_ptr<FILE, std::function<void(FILE*)>>(popen("pkgutil --pkgs", "r"), pipeDeleter);

#else
    
    std::cout << "Unsupported OS." << std::endl;
    return -1;

#endif
#elif __linux__

    auto pipeDeleter =
        [](FILE* pipe)
        {
            if (pipe == nullptr) { return; }

            pclose(pipe);
            pipe = nullptr;
        };

    pipe = std::unique_ptr<FILE, std::function<void(FILE*)>>(popen("lsb_release -d", "r"), pipeDeleter);

    std::string linuxDistro;
    if (!getPipeOutput(pipe.get(), linuxDistro))
    {
        std::cout << "Failed to get linux distro." << std::endl;
        return -1;
    }

    std::for_each(linuxDistro.begin(), linuxDistro.end(), [](char& char) { char = std::tolower(char); });

    if (linuxDistro.find("suse") != std::string::npos)
    {
        pipe.reset(popen("rpm -qa --last", "r"));
    }
    else if ((linuxDistro.find("centos") != std::string::npos) || (linuxDistro.find("red hat") != std::string::npos))
    {
        pipe.reset(popen("yum list installed", "r"));
    }
    else if (linuxDistro.find("fedora") != std::string::npos)
    {
        pipe.reset(popen("dnf list installed", "r"),);
    }
    else if (linuxDistro.find("arch") != std::string::npos)
    {
        pipe.reset(popen("pacman -Q", "r"),);
    }
    else if (linuxDistro.find("debian") != std::string::npos)
    {
        pipe.reset(popen("dpkg -l", "r"));
    }
    else if (linuxDistro.find("ubuntu") != std::string::npos)
    {
        pipe.reset(popen("apt list --installed", "r"));
    }
    else
    {
        std::cout << "Unsupported linux distro." << std::endl;
        return -1;
    }

#else

    std::cout << "Unsupported OS." << std::endl;
    return -1;

#endif

    std::string installedSoftware;
    if (!getPipeOutput(pipe.get(), installedSoftware))
    {
        std::cout << "Failed to get installed software." << std::endl;
        return -1;
    }

    std::cout << installedSoftware << std::endl;

    return 0;
}