#Overview
Hello everyone, my name is Adriano Delatorre Kairalla. First of all I'd like to point out I really enjoyed the test. It was very thoughtful and did require some research while doing it, honestly being quite fun and engaging.  I'd like to make it clear I am not a C++ expert, since the bulk of my current work is in Golang and C, but I did try my best with the knowledge I have and the research time possible for me.

### Step 1
I tried to keep the code as clean while trying to keep it in one file, but the order can get a little messy, so I apologize in advance.
I separated the assignments mostly in 3 different functions according to what they were dealing with.
For testing the variadic function, a SuperWorker class with an absurd number of hours was created.
There is a small CMake just for fun (I mostly use raw Makefiles or KBuild), my recomendation is to build it in another location not to clutter the directory, so after going to `step 1/src` folder:
```
mkdir build
cd build
cmake ../
make
```
This will create a binary called `step1`. To run the unit tests, just add the `test` parameter afterward:
```
./step1 test
```
To run the regular code just don't add anything after the binary
```
./step1
```

### Step 2
I commented heavily on the code about the parts I was unhappy about with the general issues being listed in a comment block at the top of the code.
I also heavily changed the code and added mock functions/classes in order to test the code for basic functionality, it should be compilable in any machine, but since it was a windows based code, I had to mock some things.

### Step 3
Nothing special here, just uncommented the testing code.
