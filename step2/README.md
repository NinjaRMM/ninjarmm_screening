# NINJARMM Code Review - Step 2

## Description

This program is designed to query the Windows Security Center API to retrieve information on third-party antivirus
software installed on a Windows machine. The tool uses the Windows Security Center API to determine the name of each
installed antivirus product, its version number, state, and definition state.

## Prerequisites

- Windows OS
- Visual Studio C++ compiler
- Windows SDK

## Source Code

The source code for this program is available on GitHub with Review Comments:
[CodeReview.cpp](CodeReview.cpp)

## Requirements

This program requires a C++ compiler that supports C++17, and the following Windows SDK header files:

- `<string>`
- `<map>`
- `<iostream>`
- `<windows.h>`
- `<wscapi.h>`
- `<iwscapi.h>`
- `<sddl.h>`

## Usage

To use the program, simply build and run the executable. The program will output a list of third-party antivirus
software installed on the machine, along with their version number, state, and definition state.

The function `queryWindowsForAVSoftwareDataWSC` takes a map of `std::wstring` and `AVSoftware` pairs as input and
returns a `bool` value indicating whether the query was successful or not. The function fills the map with the retrieved
data.

```c++
bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, AVSoftware>& par_avs_software_map)
```

## Code Review

The following are the suggestions to improve the code:

- The struct name `ThirdPartyAVSoftware` could be simplified to `AVSoftware` since it's already clear that it's a
  third-party software from the context.
- The naming convention for variables and functions is not consistent. It would be better to follow a consistent naming
  convention, such as camelCase or snake_case, and stick to it.
- There are inconsistent uses of `nullptr` and `NULL`. It would be better to stick to one and use it consistently
  throughout the code.
- The code is not properly indented, which can make it difficult to read.
- The function returns a boolean to indicate success or failure, but it does not provide any information about what went
  wrong if it fails.
- If an error occurs while querying an AV product, the function logs an error message but continues to process the
  remaining products. It would be better to log the error message and return immediately in such cases.
- The code uses `SysFreeString` to free the memory allocated by `get_ProductStateTimestamp`, but it does not do the same
  for the memory allocated by `get_ProductName`. It would be better to free all memory allocated by the COM functions to
  avoid memory leaks.
- The code queries the AV product list for each type of information separately, which can result in poor performance. It
  would be better to query all the information for each product in a single call to the COM functions.
- The code uses `std::wstring` for some strings, but `std::string` for others. It would be better to use `std::wstring`
  consistently throughout the code to support internationalization.
- The code could benefit from comments to explain what each section of code is doing.
- Use `nullptr` instead of `NULL`.
- Use `std::cerr` to show error messages on the console.
- Simplify the `ThirdPartyAVSoftware` struct name to `AVSoftware`.
- Use a single `ThirdPartyAVSoftware` variable instead of several individual variables.
- Add a break line to the error message to separate it from the output.
- Use the `empty` method instead of `size` to check for emptiness.
- The `Description` and `ProductState` members of the `AVSoftware` structure have the same value. Consider using empty
  strings for values not supported by Windows API query instead.
- If you want more details where exact changes were made, you can check the [CodeReview.cpp](CodeReview.cpp) file or
  history of this file.

## Changes Made

- Included headers `<string>`, `<map>`, `<iostream>`, `<windows.h>`, `<wscapi.h>`, `<iwscapi.h>`, `<sddl.h>` to fix
  several "Use of undeclared identifier" errors.
- Changed the struct name `ThirdPartyAVSoftware` to `AVSoftware` to simplify the name since it's already clear that it's
  a third-party software from the context.
- Changed the function signature of `queryWindowsForAVSoftwareDataWSC` to `std::map<std::wstring, AVSoftware>&` instead
  of `std::map<std::wstring, ThirdPartyAVSoftware>& par_avs_software_map` to have the same name used throughout the
  code.
- Simplified the code by storing all variables in one `AVSoftware` temporary variable `avs`.
- Changed the use of `NULL` to `nullptr`.
- Changed the error messages to use `std::cerr` to show the error messages on the console.

## Example

The following is an example of how to use the `queryWindowsForAVSoftwareDataWSC` function:

```c++
#include <iostream>
#include <map>
#include <string>

struct AVSoftware
{
std::wstring Name;
std::wstring Description;
std::wstring DefinitionUpdateTime;
std::string DefinitionStatus;
std::wstring Version;
std::wstring ProductState;
};

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, AVSoftware>& par_avs_software_map);

int main() {
    std::map<std::wstring, AVSoftware> avs_software_map;

    bool success = queryWindowsForAVSoftwareDataWSC(avs_software_map);

    if (success)
    {
        for (auto const& [key, val] : avs_software_map)
        {
            std::wcout << L"Name: " << val.Name << std::endl;
            std::wcout << L"Description: " << val.Description << std::endl;
            std::wcout << L"Definition update time: " << val.DefinitionUpdateTime << std::endl;
            std::wcout << L"Definition status: " << val.DefinitionStatus << std::endl;
            std::wcout << L"Version: " << val.Version << std::endl;
            std::wcout << L"Product state: " << val.ProductState << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to retrieve AV software data." << std::endl;
    }

    return 0;
}
```

