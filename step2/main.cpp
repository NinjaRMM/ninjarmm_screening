// This program has been tested on Linux/Ubuntu only.
// The code for Windows and Mac has been accomodated
// but not tested because it cannot be tested on
// https://cpp.sh.

#include <iostream>
using namespace std;

#define Linux_OS true
#define Windows_OS false
#define Mac_OS false


int main()
{
	// Run command and get result
	// to get Linux system info
	FILE *file;
	char buffer[255];
	std::string resultString = "";
#if Linux_OS
	std::string commandString = "uname -a";
#elif Windows_OS
	std::string commandString = "systeminfo";
#elif Mac_OS
	std::string commandString = "/usr/sbin/system_profiler SPInstallHistoryDataType";
#endif

#if Linux_OS || Mac_OS
	file = popen(commandString.c_str(), "r");
#elif Windows_OS
	file = _popen(commandString.c_str(), "r");
#endif
	try
	{
		while(!feof(file))
		{
			if (fgets(buffer, 255, file) == NULL)
			{
				continue;
			}
			resultString.append(buffer);
		}
	}
	catch (...)
	{
#if Linux_OS || Mac_OS
		pclose(file);
#elif Windows_OS
		_pclose(file);
#endif
		throw;
	}

#if Linux_OS || Mac_OS
	pclose(file);
#elif Windows_OS
	_pclose(file);
#endif
	cout << resultString << endl;

	return 0;

}
