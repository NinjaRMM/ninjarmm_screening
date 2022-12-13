/*
Compiled on Linux with command:
g++ -o Requirements Requirements.cpp
 
Executable: ./Requirements
*/

#ifdef __unix__
#include<iostream>
#include<sys/utsname.h>
#define OS "linux"

#elif defined(_WIN32)
#include<iostream>
#define OS "win32"

#endif

void message(){
    std::cout<<"OS is "<<OS<<", install dependencies"<<std::endl;
}

int main()
{
    if(OS == "linux") {
        message();
        system("sudo apt update");
        std::cout<<"Dependencies installed"<<std::endl;

        std::cout<<"General system information:"<<std::endl;
        system("uname -a");
    }
    if(OS == "win32") {
        message();
        system("Command to execute to install package...");
        std::cout<<"General system information:"<<std::endl;
        system("systeminfo");
    }
    return 0;
}
