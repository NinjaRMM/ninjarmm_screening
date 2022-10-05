/*
	Here we should implement mac versions of the os.hpp header
	MacOS was not tested
*/


#include <sys/sysctl.h>
//https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/gethostname.3.html
#include <unistd.h> //contains gethostname
//https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/uname.3.html
#include <sys/utsname.h>

/*
	I don't think there is a way to see how to get macOs patches programatically
*/
std::vector<OsPatch> getPatches() {
	return { };
}


long getRamMB() {
	int mib[] = { CTL_HW, HW_MEMSIZE };
	int64_t value = 0;
	size_t length = sizeof(value);
	sysctl(mib, 2, &value, &length, NULL, 0);
	return value;
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