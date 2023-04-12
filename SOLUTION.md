# Overview for the solution

Hi pals! My name is **Jaime Andres Castillo Leon** and here is my solution.

You can reach me in **jacastillol@unal.edu.co**.

## Step 1 
- The solution is compiled with `g++ -std=c++17 -I . -o the_jobs.out the_jobs.cpp`
- To run unit tests with more detail `./the_jobs.out -s`
- I have chosen `doctest` library for the unittest [doctest](https://github.com/doctest/doctest). It is easy to place test inside the production code, and installing `doctest` is just to place a unique header file in the include path.
- For the Job-Programmer-Pilot hierarchy, I've decide not to use virtual methods because for this case, my opinion, was it was not needed.
- Please follow the [commits](https://github.com/NinjaRMM/ninjarmm_screening/compare/develop...JeckyllIsHyde:ninjarmm_screening:jacl_solution), they show how was developed the solution, I tried to follow TDD.
- DoWork has an implementation do_work function that is testable.
- For the IsInBound, I create a template class with traits that reflects the LessComparableTrait
- The IsInBound has been tested with uint32_t, a custom type, and others.
- The ContainsTheString function has been implemented as a template function with a Universal Reference to a function predicate.
- Finally a second version template of ContainsTheString has been implemented as a Variadic Template, again please look at the commits, they show how the tests were refactored as the solution was getting shape.

## Step 2
For reviewing the code in `CodeReview.cpp` file, 
- Is a code for Win32 api using COM, the code style is in the middle of C and C++, most of the variables used inside `queryWindowsForAVSoftwareDataWSC` function are defined at the begining of the function.
- I prefer to move the variables close to where they are really used, it reduces the scope and this will be helpful to refactor the fuction following SOLID principles, e.g, starting in this case by Single Responsability Principle (SRP). Check [here](https://github.com/NinjaRMM/ninjarmm_screening/commit/36c71ecc4c2c609e9d54b6267e2bac74a03c5650).
- I added `PtrProduct->Release()` in several places, this probably avoid memory leaks. Check [here](https://github.com/NinjaRMM/ninjarmm_screening/commit/45333a5d6f4eb8bb196c98a6c852e95ff8795cbf).
- I would like to split the function in several functions following SRP, but Win32 code style generally uses these `HRESULT`'s to check failures, and I'd prefer to discuss with the team in a situation like this, to have a Code Style agreement on how to proceed with this refactorization. I would suggest to use `std::optional` or `std::expected`, but it depends on the compiler we use and the standard we are going to follow.
- Win32 api and COM styles often uses ugly code style with stuffs like `goto` and labels to move around a spaguetti. Check this [Microsoft example](https://github.com/microsoftarchive/msdn-code-gallery-microsoft/blob/master/Official%20Windows%20Platform%20Sample/Windows%208.1%20desktop%20samples/%5BC%2B%2B%5D-Windows%208.1%20desktop%20samples/Windows%20Security%20Center%20API%20sample/C%2B%2B/WscApiSample.cpp).

## Step 3
The solution for `CodeChallenge.cpp` file:
- To compile the solution `g++ -std=c++17 -o CodeChallenge.out CodeChallenge.cpp`.
- To run `./CodeChallenge.out`.
- I have add other template with more parameters that calls the former one, to get the `total` amount.
- Check the [commit](https://github.com/NinjaRMM/ninjarmm_screening/commit/cfaf09f7b685c94ba2de1770ac2655643820b443)

# Regards

Andres!