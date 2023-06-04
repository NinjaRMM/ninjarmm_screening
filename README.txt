# Name: Renan Leonello Castro

In this repository is my particular solution for the step1, step2 and step3 tasks.

All my solutions are inside step${1,2,3}/src directories.

In solutions step1 and step3, I tried to use only pure C++ features without adding additional
libraries like Boost, GoogleTest, Qt or any other usual library used by C++ programmers.
Because of this, I implemented the unit tests from step1 without using a unit testing framework,
and also some solutions I came up with could have been easier if I had used third-party libraries.
The reason for this is to demonstrate some of my pure C++ knowledge and also to make the solution
lighter to compile on different machines.

In solution step1, I created a CommonJob class to not have duplicated code in the implementation of the methods that would be common for the Programmer and Pilot classes.

In solution step1, I created unit tests only for the developed classes and not for all other functions
that were requested. The reason for this is that I have little time available and also I understood
that the purpose of the tasks is just to show how important unit tests are and how I, in my day to day,
develop them. So, for demonstration purposes, I made the assumption that unit testing the classes
would be sufficient to demonstrate my C++ programming skills.

In the step1 solution there is a valgrind_result.txt file that was generated executing
"valgrind -s --leak-check=full ./step1". This file shows that there are no leaks in the developed solution.

In the step2 solution there is a file called review.txt that contains the comments I would make in
a PR with the requested code. There are also .h and .cpp files that are an example on how I would
start implementing the same solution but applying the coments I did in the review.

In the step3 solution directory there is a file called "code_challeng.cpp" which contains the updated
code with the solution I chose to solve the task.

The step1 and step3 were developed in a OpenSuse Linux machine and the requirements to build the
solutions are the following:

* A PC running a Linux O.S. distribution such as Ubuntu, OpenSuse, Fedora, etc.
* C++ Compiler: You will need a C++ compiler installed on your machine. The Makefile uses g++ as
  the compiler, so make sure g++ is available in the machine.
* Standard C++ Library: The Makefile uses the C++17 standard, so the target machine should
  have the C++17 standard library installed.
* GNU Make: Ensure that your machine has GNU Make installed. Most Linux distributions come
  with Make preinstalled, but it's worth confirming.
* It's important to note that the availability and versions of these dependencies may vary
  depending on the specific Linux distribution and its package management system. However,
  in most cases, a standard Linux distribution should have these requirements available by
  default or easily installable through the package manager.
* Navigate to the step${1,3}/src and run the program "make" in a linux bash terminal.
* The make program will compile the source code and generate a binary file called "step1"
  for the first task and "step3" for the third task.
* You can execute the generated binary file by tapping "./step1" in a linux bash terminal
  inside the step1/src directory and get the results on the terminal screen.
* You can execute the generated binary file by tapping "./step3" in a linux bash terminal
  inside the step3/src directory and get the results on the terminal screen.
* The step2 exercise is note executing as it was developed to work on a Windows machine
  and my personal O.S. is Linux. I understood that the execution of this task was not a
  requirement so I didn't make it.
