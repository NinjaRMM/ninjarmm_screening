Joel Luzadas

A couple notes:

-- Step 1 --

I may have overdone this one - having read the encouragement to write additional code, I proceeded to implement an interactive command line app that prompts the user for an option, such as running one of the specified test cases, and executes it. Following are brief descriptions of the options:

	jobs   - runs test items (a) through (g), i.e. Job model tests
	bounds - runs test (h); n.b. the user will be prompted for an HTTP response code
	vector - runs test (i)
	all    - runs all the tests; n.b. the 'bounds' test will not prompt for imput
	unit   - runs the unit tests; displays only failed tests
	clear  - clears the screen
	exit   - exits the app

Note that the app successfully runs and compiles on both Windows (via Visual C++) and Linux (via GDB Online compiler, https://www.onlinegdb.com/).

I've tred to annotate the code as much as possible, but I might be light in a few places; hopefully, at least the gist got through. Please point out any areas that need further explanation.

-- Step 2 --

I didn't simply comment on the code, but did the actual changes, instead. Hopefully, the minimal comments I did leave sufficiently explain my intentions - I mainly focused on using wrapper classes to automatically manage dynamically allocated data, especially in the for-loop, where there was a high chance of memory leaks because the continue statements would've skipped the Release at the end of the loop.

