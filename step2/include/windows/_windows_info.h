#pragma once

struct SystemInfo;
#include <string>
#include <vector>



std::wstring get_windows_os();
std::wstring get_windows_RAM();
std::wstring get_windows_CPU();

bool system_get_info(SystemInfo &info);


template<typename T>
class DeleteHelper
{
public:
	DeleteHelper() = delete;
	DeleteHelper(T* ptr) : _ptr(ptr)
	{}
	~DeleteHelper()
	{
		if (_ptr)
			_ptr->Release();
	}
private:
	T* _ptr;


};




enum class WMI_OS {
	NAME,
	ARCHITECTURE
};

enum class WMI_CPU {
	NAME,
	MAXCLOCK,
	SOCKET_DESIGNATION
};

enum class WMI_RAM {
	CAPACITY,
	FORMFACTOR,
	MEMTYPE,
	SPEED
};

enum class WMI_MB {
	MANUFACTURER,
	PRODUCT
};
enum class WMI_GPU {
	ADAPTER_RAM,
	NAME
};
enum class WMI_MONITOR {
	NAME
};
enum class WMI_DISPLAYCONFIG {
	NAME
};
enum class WMI_DISKDRIVE {
	CAPTION,
	SIZE
};
enum class WMI_OPTICALDISK {
	CAPTION
};
enum class WMI_SOUND {
	CAPTION
};

enum class WMI_CPU_PERF {
	PROC_TIME_PERCENTAGE
};
