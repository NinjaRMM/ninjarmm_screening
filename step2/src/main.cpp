#include "os/os.hpp"
#include <iostream>
/*
Please write small program in C++ to do the following:
	a. Query for installed OS patches
	b. Query for general system information 
	c. Feel free to add any functionality you feel will showcase your abilities. 

*/



int main() {
    // Wasn't tested on MacOs
    // Tested on Linux and Windows
    // I would expect on a production envirorment to have tests on this outputs, 
    // that they match the expected result
    
	std::cout << "=== System Information ===" << std::endl;
	std::cout << getComputerName() << std::endl;
	std::cout << getRamMB() << " MB" << std::endl;
	std::cout << getOsName() << std::endl << std::endl << std::endl;

	std::cout << "=== System Os Patches ===" << std::endl;

	for (auto& patch : getPatches()) {
		std::cout << patch.name << " ||  " << patch.date << std::endl;
	}

    return 0;

};
