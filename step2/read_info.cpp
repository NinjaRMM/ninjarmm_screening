#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef __linux__
#include <sys/sysinfo.h>
#endif

void system_info()
{
#ifdef __linux__
	struct sysinfo info;
	::sysinfo(&info);

	constexpr std::uint64_t seconds_per_day = 24 * 60 * 60;
	constexpr std::uint64_t mb_factor = 1 << 20;

	std::cout << "uptime: " << info.uptime / seconds_per_day << std::endl;
	std::cout << "total ram (MB): " << info.totalram / mb_factor << std::endl;
	std::cout << "free ram (MB): " << info.freeram / mb_factor << std::endl;
	std::cout << "total swap (MB): " << info.totalswap / mb_factor << std::endl;
	std::cout << "free swap (MB): " << info.freeswap / mb_factor << std::endl;
	std::cout << "process running: " << info.procs << std::endl;

	auto read_sys_file = [](std::string const & info, std::string const & file)
	{
		std::ifstream ifile(file);
		if (ifile.good())
		{
			std::stringstream ss;
			ss << ifile.rdbuf();
			std::string text;
			std::getline(ss, text);
			std::cout << info << ": " << text << std::endl;
		}
	};

	read_sys_file("machine hostname",                "/proc/sys/kernel/hostname");
	read_sys_file("kernel version",                  "/proc/sys/kernel/osrelease");
	read_sys_file("os type",                         "/proc/sys/kernel/ostype");
	read_sys_file("boot unique id identifier",       "/proc/sys/kernel/random/boot_id");
	read_sys_file("date which kernel was compiled",  "/proc/sys/kernel/version");
	read_sys_file("sysrq enabled (emergency keys)",  "/proc/sys/kernel/sysrq");
	read_sys_file("max number of cgroups namespaces", "/proc/sys/user/max_cgroup_namespaces");
	read_sys_file("max number of mount namespaces",   "/proc/sys/user/max_net_namespaces");
#endif
}

void centos_patches()
{
	std::ifstream ifile("/etc/centos-release");
	if (!ifile.good())
	{
		return;
	}

	std::string cmd("rpm -q kernel --last");
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

/*!
 * @brief This code was tested in Centos7 using gcc 8.2
 * In order to take this to production WIN and MAC implementations must be providaded
 * The code will build under these OS but without any output
 */
int main()
{
	system_info();
	centos_patches();
}
