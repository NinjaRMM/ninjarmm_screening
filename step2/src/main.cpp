#include <stdio.h>
#include <iostream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>

void get_info()
{
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    printf("Hardware information: \n");  
    printf("  OEM ID: %u\n", siSysInfo.dwOemId);
    printf("  Number of processors: %u\n", siSysInfo.dwNumberOfProcessors); 
    printf("  Page size: %u\n", siSysInfo.dwPageSize); 
    printf("  Processor type: %u\n", siSysInfo.dwProcessorType); 
    printf("  Minimum application address: %lx\n", siSysInfo.lpMinimumApplicationAddress); 
    printf("  Maximum application address: %lx\n",siSysInfo.lpMaximumApplicationAddress); 
    printf("  Active processor mask: %u\n",  siSysInfo.dwActiveProcessorMask); 
}
#elif __linux__
#include<sys/utsname.h>
void get_info()
{
    struct utsname buf1;
   errno =0;
   if(uname(&buf1)!=0)
   {
      perror("uname doesn't return 0, so there is an error");
      exit(EXIT_FAILURE);
   }
   printf("System Name = %s\n", buf1.sysname);
   printf("Node Name = %s\n", buf1.nodename);
   printf("Version = %s\n", buf1.version);
   printf("Release = %s\n", buf1.release);
   printf("Machine = %s\n", buf1.machine);
}
#elif __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/ps/IOPSKeys.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <mach/mach.h>
#include <mach/processor_info.h>
#include <mach/mach_host.h>
#include <vector>
#include <stdexcept>
#include <string>

void get_info()
{

    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    mach_port_t host = mach_host_self();
    host_cpu_load_info_data_t load_info;

    kern_return_t result = host_statistics(host, HOST_CPU_LOAD_INFO, (host_info_t)&load_info, &count);

    if (result != KERN_SUCCESS)
    {
        throw std::runtime_error("An error occured while getting the cpu usage.");
    }

    int userTicks = load_info.cpu_ticks[CPU_STATE_USER];
    int systemTicks = load_info.cpu_ticks[CPU_STATE_SYSTEM];
    int idleTicks = load_info.cpu_ticks[CPU_STATE_IDLE];
    int niceTicks = load_info.cpu_ticks[CPU_STATE_NICE]; 
   
    host_basic_info_data_t info;
    result = host_info(host, HOST_BASIC_INFO, (host_info_t)&info, &count);
  
    printf("Total Memory %u b\n",info.memory_size); 
    printf("Total Memory %llu b\n",info.max_mem);   
    printf("User cpu ticks usage %d \n",userTicks) ;     
    printf("System cpu ticks usage %d\n",systemTicks) ;     
}
#else
void get_info(){
    printf("Error. Unknown system\n")
}
#endif


int main()
{
    
    /* Getting system information. 
    Tested on Linux e MacOs. Not tested on Windows  */

    get_info();

    return 0;
}