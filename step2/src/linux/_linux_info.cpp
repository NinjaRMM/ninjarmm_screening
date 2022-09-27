#include "_linux_info.h"
#include "system_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/utsname.h>

// this module is private for windows. I create a project with Code::clocks for linux. With cmake we need add this module only in linux.

bool system_get_info(SystemInfo &info)
{

    FILE* pip;
    char buffer[1024];
    pip = popen("cat /proc/cpuinfo | grep 'model name' | uniq |"
                " sed 's/model name[[:blank:]]*:[[:blank:]]*//' | awk 1 ORS=' '", "r");

    if(NULL == pip)
    {
        perror ("¡Popen failure!");
        return false;
    }
    info.cpu = fgets(buffer, sizeof(buffer), pip);
    pclose(pip);

    pip = popen("cat /proc/meminfo | grep 'MemTotal' | sed 's/MemTotal:[[:blank:]]*//'"
                " | awk '{$1=$1/1024^2; print $1,\"GB\";}' | awk 1 ORS=' '", "r");
    if(NULL == pip)
    {
        perror ("¡Popen failure!");
        return false;
    }
    info.memory = fgets(buffer, sizeof(buffer), pip);

    pclose(pip);
    //struct utsname unameData;
    //uname(&unameData);

    pip = popen("lsb_release -a", "r");
     if(NULL == pip)
    {
        perror ("¡Popen failure!");
        return false;
    }

    while(fgets(buffer, sizeof(buffer), pip)!=NULL)
    {

        char name[256];
        char distro[256];
        if(!(sscanf(buffer,"%s %[^\n]", name,distro)==2))
            continue; // log this error

        if(strcasecmp(name,"Description:")==0)
        {
            info.os=distro;
            break;
        }

    }
    pclose(pip);


    return true;

}
