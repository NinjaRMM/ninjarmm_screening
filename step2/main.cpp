// This program was only compile on https://cpp.sh as I do not have a local compiler installed on my machine
// I did some research on existing applications for windows, linux and apple that could provide the appropriate information.
// I'm fairly confident it would execute the appropriate command and display the desired output.
// Ideally I would want a VM for each of the platforms to run this on to fully validate it.

#include <iostream>

using namespace std;

int main()
{
    FILE* file;
    char buffer[255];
    std::string result = "";

#if defined(_WIN32) || defined(_WIN64)
    std::string cmd = "systeminfo";
#elif defined(__APPLE__)
    std::string cmd = "/usr/sbin/system_profiler SPInstallHistoryDataType";
#elif defined(__linux__)
    std::string cmd = "systeminfo";
#endif

#if defined(_WIN32) || defined(_WIN64)
    file = _popen(cmd.c_str(), "r");
#elif defined(__APPLE__) || defined(__linux__)
    file = popen(cmd.c_str(), "r");
#endif

    try
    {
        while(!feof(file) && file != nullptr)
        {
            if(fgets(buffer, 255, file) == NULL)
            {
                continue;
            }
            result.append(buffer);
        }
    }
    catch(...)
    {
#if defined(_WIN32) || defined(_WIN64)
        _pclose(file);
#elif defined(__APPLE__) || defined(__linux__)
        pclose(file);
#endif
        throw;
    }

#if defined(_WIN32) || defined(_WIN64)
    _pclose(file);
#elif defined(__APPLE__) || defined(__linux__)
    pclose(file);
#endif

    cout<<result<<endl;

    return 0;
    
}