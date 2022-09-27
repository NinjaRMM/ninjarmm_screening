#include "patch_info.h"
#include "_linux_patches.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <errno.h>
#include <string.h>


bool system_get_patches(std::vector<PatchInfo> &patches)
{

    FILE *in;
    char buff[1024];

    if(!(in = popen("zcat /var/log/dpkg.log.*.gz | cat - /var/log/dpkg.log | grep -E 'install |upgrade |remove'", "r")))
        return false;


    while(fgets(buff, sizeof(buff), in)!=NULL)
    {
        char date[64];
        char hour[64];
        char type[8];
        char name[256];
        if(!(sscanf(buff,"%s %s %s %s", date,hour,type,name)==4))
            continue; // log this error

        PatchInfo patch;
        patch.name=name;
        patch.type_description=type;
        if(strcasecmp(type,"install")==0)
            patch.type=PatchType::Install;
        else if(strcasecmp(type,"upgrade")==0)
            patch.type=PatchType::Upgrade;
        else // remove case
            patch.type=PatchType::Remove;

        patches.push_back(patch);

    }
    pclose(in);

    return true;
}

