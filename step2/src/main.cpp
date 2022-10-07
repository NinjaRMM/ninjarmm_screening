/**
 * This application should run on Ubuntu Linux. It
 * will likely work on any linux distro that uses dpkg to
 * manager packages.
 *
 * The app has one command line argument that can contain the following values:
 *      packages: display all installed packages
 *      sysinfo: display information about the system
 */
#include <cstdio>
#include <string>
#include <array>
#include <memory>
#include <iostream>
#include <sstream>
#include <functional>
#include <map>
#include <sys/sysinfo.h>

/**
 * @fn execute
 *
 * @brief Executes the passed command and stores the output in string
 *
 * @param command The command to execute
 * @param output The string to stuff the output into
 *
 * @return true on success
 */
bool execute(const char *command, std::string &output) {
    std::array<char, 512> buffer;

    /**
     * If we have invalid arguments, fail
     */
    if (command == nullptr) {
        return false;
    }

    /**
     * open our pipe and execute our command. store the FILE* in a unique pointer
     * with a lambda as the deleter. Yes, i could have used decltype here instead of
     * defining the signature and writing a lambda ... but well... i wanted to write the
     * lambda.
     */
    std::unique_ptr<FILE, void (*)(FILE *stream)> outputPipe(
            popen(command, "r"),[](FILE *stream) {
                pclose(stream);
            });
    if (!outputPipe) {
        return false;
    }

    /** now read the result into our output string */
    while (std::fgets(buffer.data(), buffer.size(), outputPipe.get())) {
        output.append(buffer.data());
    }

    return true;
}

/**
 * @fn displayInstalledPackages
 *
 * @brief Display the list of installed package names on this system
 *
 * @return true on success
 */
bool displayInstalledPackages() {
    static const char *command = "dpkg --get-selections";

    std::string commandOutput;

    /**
     * Execute apt and list all te installed packages
     */
    if (!execute(command, commandOutput)) {
        return false;
    }

    /** wrap our output in a stringstream */
    std::stringstream ss(commandOutput);

    /** crack out some lines */
    std::string line;
    while (std::getline(ss, line)) {
        /** we really only want the name */
        std::stringstream lineStream(line);

        std::string name;
        lineStream >> name;

        std::cout << name << std::endl;
    }

    return true;
}

/**
 * @fn displaySystemInfo
 *
 * @brief Display basic system info about the system.
 *        Kernel version, RAM, processor count.
 * @return
 */
bool displaySystemInfo() {
    bool rc = true;

    static const char *cmdSystemName = "uname -n";
    static const char *cmdKernelVersion = "uname -sr";
    static const char *cmdArchitecture = "uname -m";
    static constexpr double megabyte = 1048576.0f;

    std::stringstream ss;
    std::string systemNameOutput;
    std::string kernelVersionOutput;
    std::string architectureOutput;
    std::string systemName;
    std::string kernelVersion;
    std::string architecture;
    double totalMemory = 0.0f;
    double freeMemory = 0.0f;
    unsigned short processCount = 0;

    rc = rc && execute(cmdSystemName, systemNameOutput);
    rc = rc && execute(cmdKernelVersion, kernelVersionOutput);
    rc = rc && execute(cmdArchitecture, architectureOutput);

    if (rc) {
        ss.str(systemNameOutput);
        ss >> systemName;
        ss.str(kernelVersionOutput);
        ss >> kernelVersion;
        ss.str(architectureOutput);
        ss >> architecture;

        /** now lets grab some info about memory */
        struct sysinfo si;
        if (sysinfo(&si) == 0) {
            uint64_t tMem = (uint64_t)si.totalram * (uint64_t)si.mem_unit;
            uint64_t fMem = (uint64_t)si.freeram * (uint64_t)si.mem_unit;
            totalMemory = (double)tMem / megabyte;
            freeMemory = (double)fMem / megabyte;
            processCount = si.procs;
        } else {
            rc = false;
        }
    }

    if (rc) {
        std::cout.precision(1);
        std::cout << "Name: " << systemName << std::endl;
        std::cout << "Kernel: " << kernelVersion << std::endl;
        std::cout << "Architecture: " << architecture << std::endl;
        std::cout << std::fixed;
        std::cout << "Total RAM(MB): " << totalMemory << std::endl;
        std::cout << "Free RAM(MB): " << freeMemory << std::endl;
        std::cout << "Process Count: " << processCount << std::endl;
    }

    return rc;
};

/**
 * @fn Usage
 *
 * @brief Display usage info and return an error code.
 *
 * @return 1 for failure
 */
int usage() {
    std::cout << "Usage: step2 [packages|sysinfo]" << std::endl;
    std::cout << "  packages: display all installed package names" << std::endl;
    std::cout << "  sysinfo: display basic system info" << std::endl;
    return 1;
}

/**
 * Entry point
 *
 * Compiled and tested on Ubuntu 22.04 using the clang compiler.
 * Of course this code isn't production ready. The formatting of
 * the output is pretty basic. You might want to pretty it up
 * with some by creating a table for the system info and
 * for the packages you could query more than just the name
 * and do the same.
 *
 * Of course you wouldnt REALLY do any ot that on a linux machine because who
 * is going to use this program when they have:
 *      apt
 *      free
 *      uname
 * And a whole host of other utilities that do this much better.
 *
 */
int main(int argc, char **argv) {
    /* if you want to add more options add a function above and add an entry int the map */
    static std::map<std::string, std::function<bool ()>> operations = {
        { "packages", [] { return displayInstalledPackages(); } },
        { "sysinfo", [] { return displaySystemInfo(); } }
    };

    if (argc == 2) {
        auto operation = operations.find(argv[1]);
        if (operation != operations.end()) {
            return operation->second() ? 0 : 1;
        }
    }

    return usage();
}
