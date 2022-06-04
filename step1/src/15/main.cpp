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

    pipe  = std::unique_ptr<FILE, std::function<void(FILE*)>>(_popen("systeminfo.exe", "r"), pipeDeleter);

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

    pipe = std::unique_ptr<FILE, std::function<void(FILE*)>>(popen("system_profiler SPHardwareDataType", "r"), pipeDeleter);

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

    pipe = std::unique_ptr<FILE, std::function<void(FILE*)>>(popen("sudo lshw", "r"), pipeDeleter);

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