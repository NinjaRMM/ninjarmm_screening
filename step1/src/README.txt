Explanation for each item, so the interviewer can better understand my mindset:
* All my comments is prefixed with the sequence: '->'

How to compile the code:
  g++ -std=c++11 solution01.cpp -o solution01


1. Write a small program in C++ that will contain the following:
	a. Create a class Job
           -> As the exercise asks for creating an interface, the "Job" class was created using only pure virtual methods, so the subclasses needs to implements each method.

	b. Create a subclass Programmer
           -> The subclasses implements specific behaviours for each kind of "Job"

	c. Create a subclass Pilot
           -> As the Pilot is another kind of "Job", the idea of specific behaviours was implemented on this class.

	d. The job class should provide an interface that can do the following
		i. Get the name of the job
		ii. Get the description of the job. 
		iii. Get the number of hours required by the job
		iv. A DoWork function that prints out “My work involves ” + the description.  
           -> As explained on item (a), all the methods were provided on the interface in the "Job" class, and they needs to be implemented on a specific subclass.

	e. Dynamically allocate each type of job.
           -> It was done, trivially, and doesn't need any explanation.

	f. Store each instance in a collection.
           -> Both objects were stored in a std::list

	g. Iterate the collection and call each of the available interface functions.
           -> Ok, it was done.

	h. Create a template function that will test if the provided values are within a range. Call the function and print out the inputs and result. The following should execute assuming httpResonse is a uint32_t value:
		i. IsInBounds<uint32_t>(httpResonseCode, 500, 599)
           -> Task completed.

	i. Create a function that will take a vector of strings and a test function, It should return how many items in the collection matched the test. Then print out the result.
		i. auto theStrings = vector<std::string> { “one”, “two”,  “test”};
		ii. auto count = ContainsTheString([](const std::string& tested) { return tested == “test” }, theStrings);
           -> Task completed using lambda function.

	j. Write any other code you feel would show off your understanding of C/C++ and programming in general. Bonus: Variadic Template /w usage. 
           -> I have written two codes:
              -> a) A variadic function to sum parameters
              -> b) A class to create windows on a text user interface, with colors, using the ANSI capability of terminals.

	k. Please add passing unit tests. 
           -> I've created a new template function to test the sum function.

	l. All The source can be in 1 file, it should compile and there should be no runtime errors, and no memory leaks. Use built in types freely to help you manage resources. 
           -> The source code is in 1 file, and I've used valgrind to test the solution preventing memory leaks.

