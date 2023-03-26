Full name: Iban Rodríguez Iglesias

Step1 and Step2 are the only two practices which must compile.
To compile I have used CMake. That will ensure portability.

Step1 has tests writen in GTest. Do not worry, all the dependencies 
will be intalled during the intallation process.

I would never write code with that
distribution. I prefer a src, include, 
tst directory tree (like the one used in
https://github.com/errowdrigorena/numberTranslator/tree/main/numberTranslator).
And I also prefer using namespaces, like I do there.
But as it was put that I could write everithing in one file I have decided
to use 2 files, just to make it easier to write tests, a .hpp file and a .cpp file.
The full logic is in the .hpp file.
An aditional file is uses just for testing purposes.

How to install
**************
Ensure that you have a working and configured CMake and you are connected to the Internet.
Clone this repository.
Open a terminal, or terminal like, and go to the numberTranslator folder
which is at the level of the Readme.txt file (this file), once ther move to step1 or step3 folders.
Once there:
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .

How to use
***********
After the installataion in the directory you are (build) a CodeChallenge or 
Setep1 and Step1Tests executable can be found.
Now you just have to execute them. For example: $ ./Step1

Step1: corresponds to step1 exercise whith requirements.
Step1Tests: tests for those requirements using GTest.
CodeChallenge: step3 exercise. To stimulate the new code
    uncoment the hint, as if you do not stimulate checkIfPositive
    with 5 arguments that code will not be compiled.
