// Step2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "sdkddkver.h"
#include <iostream>
#include <ShlObj.h>
#include <stdio.h>
#if defined(__APPLE__)
    #include <unistd.h>
#endif
#include <atlbase.h>
#include <fstream>
#include <atlstr.h>

enum OS_flavor { unknown, windows, apple, linux };

OS_flavor determine_os()
{
    //Detect the O/S
#if defined(_WIN32)
    std::cout << "Windows 32 or 64 bit operating system detected.\n"; //should be defined for both 32 and 64 bit
    return windows;
#elif defined(_WIN64) //just defined for 64 bit
    std::cout << "Windows 64 bit operating system detected.\n";
    return windows;
#elif defined(__APPLE__)
    std::cout << "Apple operating system detected.\n"; //should be defined for both 32 and 64 bit
    return apple;
#elif defined(__linux__)
    std::cout << "Linux operating system detected.\n"; //should be defined for both 32 and 64 bit
    return linux;
#else
    std::cout << "Not able to detect operating system.\n";
    return unknown;
#endif
}

void PrintWindowsPatches()
{
    HRESULT hr = CoInitialize(NULL);
    int count = 0;

    if (SUCCEEDED(hr))
    {
        std::cout << "About to list Windows updates:\n";
        CComPtr<IShellItem> pUpdates;
        CComPtr<IEnumShellItems> pShellEnum;

        hr = SHGetKnownFolderItem(FOLDERID_AppUpdates, static_cast<KNOWN_FOLDER_FLAG>(0), nullptr, IID_PPV_ARGS(&pUpdates));
        hr = pUpdates->BindToHandler(nullptr, BHID_EnumItems, IID_PPV_ARGS(&pShellEnum));
        if (pShellEnum)
        {
            do {
                CComPtr<IShellItem> pItem;
                CComHeapPtr<WCHAR> update_name;

                hr = pShellEnum->Next(1, &pItem, nullptr);
                if (pItem)
                {
                    HRESULT hres = pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &update_name);
                    //std::cout << static_cast<CString>(update_name) << '\n';
                    std::cout << static_cast<LPWSTR>(update_name) << '\n';
                    count++;
                }
            } while (hr == S_OK);
        }
    }
    CoUninitialize();
}

void PrintWindowsSystemInfo()
{
    SYSTEM_INFO siSysInfo;

    GetSystemInfo(&siSysInfo);

    printf("Hardware information: \n");
    printf("  OEM ID: %u\n", siSysInfo.dwOemId);
    printf("  Number of processors: %u\n",
        siSysInfo.dwNumberOfProcessors);
    printf("  Page size: %u\n", siSysInfo.dwPageSize);
    printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
    printf("  Minimum application address: %lx\n",
        siSysInfo.lpMinimumApplicationAddress);
    printf("  Maximum application address: %lx\n",
        siSysInfo.lpMaximumApplicationAddress);
    printf("  Active processor mask: %u\n",
        siSysInfo.dwActiveProcessorMask);
}

void PrintApplePatches()
{
#if defined(__APPLE__)
    struct osver {
        int minor;
        int sub;
    };

    char* darwinversion = new char[200];
    osver* osxversion = new osver;
        /*
            From the book Mac Os X and IOS Internals:
            In version 10.1.1, Darwin (the core OS) was renumbered from v1.4.1 to 5.1,
            and since then has followed the OS X numbers consistently by being four
            numbers ahead of the minor version, and aligning its own minor with the
            sub-version.
        */
        char firstelm[2] = { 0,0 }, secElm[2] = { 0,0 };

        if (strlen(darwinversion) < 5) {
            fprintf(stderr, "%s: %s Can't possibly be a version string. Exiting\n", __PRETTY_FUNCTION__, darwinversion);
            fflush(stdout);
            exit(2);
        }
        char* s = darwinversion, * t = firstelm, * curdot = strchr(darwinversion, '.');

        while (s != curdot)
            *t++ = *s++;
        t = secElm;
        curdot = strchr(++s, '.');
        while (s != curdot)
            *t++ = *s++;
        int maj = 0, min = 0;
        maj = (int)strtol(firstelm, (char**)NULL, 10);
        if (maj == 0 && errno == EINVAL) {
            fprintf(stderr, "%s Error during conversion of version string\n", __PRETTY_FUNCTION__);
            fflush(stdout);
            exit(4);
        }

        min = (int)strtol(secElm, (char**)NULL, 10);

        if (min == 0 && errno == EINVAL) {
            fprintf(stderr, "%s: Error during conversion of version string\n", __PRETTY_FUNCTION__);
            fflush(stdout);
            exit(4);
        }
        osxversion->minor = maj - 4;
        osxversion->sub = min;
        std::cout << "Darwin version is " << darwinversion << "\n";
        std::cout << "OS version is " << osxversion->minor << "." << osxversion->sub << "\n";
#endif
}

void PrintAppleSystemInfo()
{
#if defined(__APPLE__)
    int mib[4];
    int numCPU;
    std::size_t len = sizeof(numCPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &numCPU, &len, NULL, 0);

    if (numCPU < 1)
    {
        mib[1] = HW_NCPU;
        sysctl(mib, 2, &numCPU, &len, NULL, 0);
        if (numCPU < 1)
            numCPU = 1;
    }
#endif
}

void PrintLinuxPatches()
{
#if defined(__APPLE__)
        int buff_size = 32;
        char* buff = new char[buff_size];

        char* ret = NULL;
        CString str = "";

        int fd[2];
        int old_fd[3];
        pipe(fd);


        old_fd[0] = dup(STDIN_FILENO);
        old_fd[1] = dup(STDOUT_FILENO);
        old_fd[2] = dup(STDERR_FILENO);

        int pid = fork();
        switch (pid) {
        case 0:
            close(fd[0]);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            dup2(fd[1], STDOUT_FILENO);
            dup2(fd[1], STDERR_FILENO);
            system(cmd);
            //execlp((const char*)cmd, cmd,0);
            close(fd[1]);
            exit(0);
            break;
        case -1:
            cerr << "GetSystemOutput/fork() error\n" << endl;
            exit(1);
        default:
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);

            int rc = 1;
            while (rc > 0) {
                rc = read(fd[0], buff, buff_size);
                str.append(buff, rc);
                //memset(buff, 0, buff_size);
            }

            ret = new char[strlen((char*)str.c_str())];

            strcpy(ret, (char*)str.c_str());

            waitpid(pid, NULL, 0);
            close(fd[0]);
        }

        dup2(STDIN_FILENO, old_fd[0]);
        dup2(STDOUT_FILENO, old_fd[1]);
        dup2(STDERR_FILENO, old_fd[2]);
#endif
}

void PrintLinuxSystemInfo()
{
    char buffer[5000];

    system("top > output");

    std::ifstream output("output");
    output.read(buffer, 5000);
    std::cout << buffer << '\n';

    system("rm output");
}

int main()
{
    OS_flavor operating_sys_flavor = determine_os();
    if (operating_sys_flavor == windows)
    {
        PrintWindowsPatches();
        PrintWindowsSystemInfo();
    }
    else if (operating_sys_flavor == apple)
    {
        PrintApplePatches();
        PrintAppleSystemInfo();
    }
    else if (operating_sys_flavor == linux)
    {
        PrintLinuxPatches();
        PrintLinuxSystemInfo();
    }
}