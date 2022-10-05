/*
	Here we should implement linux versions of the os.hpp header



*/
#include <sys/utsname.h>
#include <sys/sysinfo.h>


/*
	Getting the OS patches installed is basically listing kernel patches?
	There is no generic way of knowing how many available patches on linux,
	Maybe we could query apt for debian based linuxes,
*/
std::vector<OsPatch> getPatches() {
	return { };
}


long getRamMB() {
	struct sysinfo info = {};
	sysinfo(info);
	return info.totalram / 1024 / 1024;
}

std::string getComputerName() {
	char hostname[HOST_NAME_MAX];
	gethostname(hostname, HOST_NAME_MAX);
	return hostname;
}

std::string getOsName() {
	utsname name = {};
	uname(&name);
	std::cout << name.version;
}