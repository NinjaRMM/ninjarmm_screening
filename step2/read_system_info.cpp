#include <cstring>
#include <iostream>

#ifdef __linux__
#include <sys/sysinfo.h>
#endif

int main()
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
#endif

	return 0;
}
