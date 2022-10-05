
#include <string>
#include <vector>

struct OsPatch {
	std::string name;
	std::string date;
};


std::vector<OsPatch> getPatches();

long getRamMB();
std::string getComputerName();
std::string getOsName();

