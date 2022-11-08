#include <cstdlib>
#include <ctype.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Helper function to convert string to upper case
string toUpper(string source) {
  string result;
  for(int i = 0; i < source.length(); i++) {
      result += toupper(source[i]);
  }
  return result;
}

/* I was not able to get my system set up and was unable to test the system() calls on any of the operating systems.  This program
 * requests the user to input which operating system they are running and runs a commandline to access the system information.
 * As I could not get this to run properly on my machine, I did not try auto-detect the operating system or attempt to process the
 * command line output in any way.  Testing this on the https://cpp.sh/ I had to comment out the system call as I believe it to not
 * be supported.  Before taking this out to production, I would like for the program to not require user input and auto detect the
 * OS as well as to confirm the purpose of this program, what type of output formats would be preferred and specific information to
 * filter out.  For the submission, system commands have been un-commented.
 */

int main()
{
  enum {
    WINDOWS_OS,
    LINUX_OS,
    MAC_OS,
    TOTAL_OS,
    UNKNOWN_OS,
  };
  
  const vector<string> OS_NAMES = {"WINDOWS", "LINUX", "MAC"};
  const string WINDOWS_COMMAND ("systeminfo");
  const vector<string> LINUX_COMMANDS  = {"uname -s", "apt list -upgradable | grep \"-security\""};
  const string MAC_COMMAND ("system_profiler SPSoftwareDataType SPInstallHistoryDataType");
  string response;
  int osType = UNKNOWN_OS;
  bool complete = false;
  
  while (!complete) {
    cout << "Which operating system are you running:" << endl;
    for (int i = 0; i < OS_NAMES.size(); i++) {
      cout << i + 1 << ". " << OS_NAMES[i] << endl;
    }
    getline (cin, response);
    response = toUpper(response);

    for (int i = 0; i < TOTAL_OS; i++) {
      string numberSelection = to_string(i + 1);
      if ((response.find(OS_NAMES[i]) != string::npos) || (response.find(numberSelection) != string::npos)) {
        osType = i;
        break;
      }
    }
  
    switch(osType) {
      case WINDOWS_OS:
        cout << "Running: " << WINDOWS_COMMAND << endl;
        system(WINDOWS_COMMAND);
        complete = true;
        break;
        
      case LINUX_OS:
        for (int i = 0; i < LINUX_COMMANDS.size(); i++) {
          cout << "Running: " << LINUX_COMMANDS[i] << endl;
          system(LINUX_COMMANDS[i]);
          complete = true;
        }
        break;
        
      case MAC_OS:
        cout << "Running: " << MAC_COMMAND << endl;
        system(MAC_COMMAND);
        complete = true;
        break;
        
      default:
        cout << "Operating system response not recognized, please choose one of the options" << endl;
        break;
    }
  }
  
  return 0;
}
