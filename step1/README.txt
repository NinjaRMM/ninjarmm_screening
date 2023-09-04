Hi, 

In order to compile the code easily, I let a CMakeList.txt file in the src directory.
If you are using linux, execute the script build.sh in the same directory. Otherwise use the script as a template.
The commands basically do the following steps:

 > create a directory "build" inside "src"
 > enter in "build"
 > execute "cmake .." and "make"

After the compilation, you can manually run the executables:
 > run the file "src/build/step1" to see the output of main.cpp with the answer of the requirements
 > run the file "src/build/tests" to see the output of the unit tests implemented in tests.cpp
