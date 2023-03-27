Stephen William David Pilcher

For step 1, I set up a simple CMake config
This calls for a C++17 compliant compiler

Step 3 also has a CMake config, but doe not require C++17
While it should be possible to also add an overload that permits a 3-parameter call with 2 ints and a total,
that was not requested. Assuming the requirement has already been refined with the Product Owner,
I implemented it to only support the 4-parameter variant requested by the validity section.
