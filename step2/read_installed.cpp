#include <cstring>
#include <iostream>
#include <filesystem>

void centos()
{
	std::string cmd("yum list installed 2>&1");
	FILE * pipe = popen(cmd.c_str(), "r");

	if (!pipe)
	{
		return;
	}

	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	while(fgets(buffer, sizeof(buffer), pipe))
	{
		std::cout << buffer;
	}

	pclose(pipe);
}

int main()
{
	if (std::filesystem::exists("/etc/centos-release"))
	{
		centos();
	}
	return 0;
}
