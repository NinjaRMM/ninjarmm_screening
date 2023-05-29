Author: Ivo Machado de Souza

## Code Building

For steps 1 and 3, CMake and a C++20 compatible toolchain are required to compile the code.
For step 2, CMake and a Visual Studio toolchain are required to compile the code.

To build it, run the following commands:
    # inside step1, step2 or step3 directories
    mkdir build
    cd build
    cmake ..
    cmake --build .

The executables should be right inside the build folder on Linux or in the build/Debug folder on windows.

### Step1

I implemented two executables: step1 and test_step1 that run on Linux and Windows.
The `step1` has the implementation of the Requirements points from `e` to `i`.
The `test_step` runs unit tests.

I used GoogleTest and GoogleMock for testing and the build requires an internet connection to download them.

### Step2

There is a `step2` executable that only runs on Windows due to the Win32 dependency.
I made a few changes regarding style and resource management that are described in comments.

### Step3

There is a `step3` executable that runs on both Windows and Linux compiled with the CodeChallenge.cpp source code.
I solved the problem using variadic templates but allowing only one parameter as an int reference and assigning the total to it.
I thought about using std::is_same to make sure that the function would only work with an int and not a variation of int but
I preferred to only change the section marked as editable and not include the <type_traits> header.
