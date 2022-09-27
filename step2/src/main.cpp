#include <iostream>
#include <system_info.h>
#include <patch_info.h>
#ifdef __linux__
#include <_linux_info.h>
#include <_linux_patches.h>
#elif _WIN32
#include <_windows_info.h>
#include <_windows_patches.h>
#else

#endif

// code was compiled and tested for linux (ubuntu) and windows 10 pro. I don´t have a MAC and therefore i don´t have MAC code.
// in windows i used visual studio and in linux i used code blocks and gcc compiler.
// to production code we need test this code for the different linux distros and architectures supported. //For windows the same.
int main()
{
    SystemInfo info;
    std::vector<PatchInfo> patches;
#ifdef __linux__
    bool res=system_get_info(info);
    std::cout << "linux info:" << std::endl;
    if(res)
    {
        std::cout << "memory: "  << info.memory << std::endl;
        std::cout << "CPU: "<< info.cpu << std::endl;
        std::cout << "OS: "<< info.os << std::endl;

    }
    std::cout << std::endl;

    std::cout << "linux patches:" << std::endl;
    res=system_get_patches(patches);
    if(res)
    {
        for(const auto &it:patches)
        {
            std::cout << "patch name: "<< it.name << "  ("<< it.type_description << ")" << std::endl;
        }
    }
#elif _WIN32
    try
    {
        std::cout << "windows info:" << std::endl;
        bool res = system_get_info(info);
        if (res)
        {
            std::cout << "OS: " << info.os << std::endl;
            std::cout << "memory: " << info.memory << std::endl;
            std::cout << "CPU: " << info.cpu << std::endl;
        }
        std::cout  << std::endl;

        std::cout << "windows patches:" << std::endl;

        res = system_get_patches(patches);
        if (res)
        {
            for (const auto& it : patches)
            {
                std::cout << "patch name: " << it.name << "  (" << it.type_description << ")" << std::endl;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }
#else

#endif

    return 0;
}
