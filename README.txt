Author: Ivo Machado de Souza

## Code Building

For steps 1 and 3, CMake and a C++20 compatible toolchain are required to compile the code.
For steps 2, CMake and a Visual Studio toolchain are required to compile the code.

To build it, run the following commands:
    # inside step1, step2 or step3 directories
    mkdir build
    cd build
    cmake ..
    cmake --build .

The executables should be right inside the build folder in Linux or in the build/Debug folder in windows.

### Step1

I implemented two executables: step1 and test_step1 that run on Linux and Windows.
The `step1` has the implementation of Requirements points from `e` to `i`.
the `test_step` run unit tests.

I used GoogleTest and GoogleMock for testing the the build requires internet connection to download them.

I didn't use any variadic template here because I couldn't see a good application for it.

### Step2

There is a step2 executable that only runs on Windows due to the Win32 dependency.
I made a few changes regarding style and resource management that are described in comments.

### Step3

There is a step3 executable that runs on both Windows and Linux compiled with the CodeChallenge.cpp source code.
I solved the problem using variadic templates but allowing only one parameter as a int reference and assigning the total to it.
