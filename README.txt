Author: Gabriel Sales Lins Rodrigues
Linkedin: https://www.linkedin.com/in/gabrielslrodrigues/
Email: gabriel.rodrigues@freiheitech.com

# Step 1
Even though it was stated that the whole source could be in one file, I tried to setup a folder structure similar to what I would do in a larger project.
So header files go into the 'include' folder, source files into 'src', 'apps' folder contain the main application, and unit tests are in the 'tests' folder.
I used CMake so, to build and execute the source code, follow the steps:
- Go into the step1/src folder
- Run the command: cmake -S . -B build
- To build the main application and unit tests, run: cmake --build build
- To run the main application: ./build/apps/app
The main app covers items 'a' to 'j'. Regarding item 'j', I replicated a similar function to item 'i', but using a variadic template.
For item 'k', I've used GoogleTest, which is fetched by CMake. To run the unit tests, do this:
- Run the command: ./build/tests/unittests

# Step 2
I've added comments throughout the file pointing out necessary libraries to include. I've also edited the file to include such libraries.
The code itself depends on the Win32 API and seems to loop through AV products, getting their info (name, description, version, etc.) , and storing it in a
map of struct objects which was passed by reference to the function.

# Step 3
Similar to step3, I've also used CMake in this step. To execute the code, steps are as follows:
- Go into the step3/src folder
- Run the command: cmake -S . -B build
- To build the application, run: cmake --build build
- To run the main application: ./build/step3
In this step, I chose to create an overload of the template function that takes the 'total' as a new parameter by reference. This overload computes the total
and then calls the original template function to return its output.

Thanks for the time reviewing my answers!
