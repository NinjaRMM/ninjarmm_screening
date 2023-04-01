Name: Henrique Awoyama Klein

Hi, I'm a computer engineer with nearly 10 years of software development, mainly focused on embedded systems.
I have 6+ years in C++ development, both in embedded and non-embedded systems using QT.

In this test I added google test as a submodule to enable unit testing on step1. 
On every code I write I use clang-format with a specific set of parameters I'm used to.
As a standard I add doxygen documentation, although I just build it occasionally.
I used VS Code and g++ version 11.3.0 for thi exercise, running on a Linux Mint.

For step 1 I create a build directory within step1/src/ and call "make" and run with "./step1Tests". 
I added "build" directory to the repository even though it wasn't necessary.


For step 2 I though of 3 approaches. The first is actually using a plugin that is non intrusive, which means it 
just outputs a .csv file. However since this is dependent for the user to have this plugin I didn't go forward with it.
I ended up basically rewriting everything I saw fit, but haven't changed anything (hopefully) on the inner workings, 
which means the function should work the same way (except the return type which I though it'd be better changing)
I also uploaded a third alternative which is just commenting how I would change things up (which is what I did on the 
actual file).

For the step 3 I just uncommented the code that should work after I solved the issue. I put a simple Makefile there 
to test it even though is quite simple.

