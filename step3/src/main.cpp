//**************************************************************
// Sam Thomsen
// NinjaOne Code Screening
// January 2023
//
// This should build for Windows, macOS and Linux with the 
// provided makefiles. Windows requires MS VC++ (cl) and both 
// Linux and macOS require g++.
//
//**************************************************************

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <vector>

//**************************************************************
// class Job
//**************************************************************

class Job
{
public:

    Job( int hours ):
        _hours( hours )
    {
    }

    virtual std::string name() const
    {
        // return
        return "Job";
    }

    virtual std::string description() const
    {
        // return
        return "performing generic tasks";
    }

    int hoursRequired() const
    {
        // return
        return _hours;
    }

    std::string doWork() const
    {
        // return
        return "My work involves " + description();
    }

private:

    // hours
    int _hours = 0;
};

//**************************************************************
// class Programmer
//**************************************************************

class Programmer : public Job
{
public:

    Programmer( int hours ):
        Job( hours )
    {
    }

    std::string name() const override
    {
        // return
        return "Programmer";
    }

    std::string description() const override
    {
        // return
        return "programming computers to do cool things";
    }
};

//**************************************************************
// class Pilot
//**************************************************************

class Pilot : public Job
{
public:

    Pilot( int hours ):
        Job( hours )
    {
    }

    std::string name() const override
    {
        // return
        return "Pilot";
    }

    std::string description() const override
    {
        // return
        return "flying airplanes to interesting places";
    }
};

//**************************************************************
// bool IsInBounds()
//
// Range extents do not need to be the same type as the code.
// Template will match as long as type B is castable to type A
//**************************************************************

template<typename A, typename B>
bool IsInBounds( A httpResonseCode, B from, B to )
{
    // inputs
    std::cout << "Inputs: httpResponseCode: " << httpResonseCode << " from: " << from << " to: " << to << "\n\n";

    // result
    bool result = ( httpResonseCode >= A( from )) && ( httpResonseCode <= A( to ));

    // output
    std::cout << "Value is " << (( result ) ? "" : "not " ) << "in range.\n\n";

    // return
    return result;
}

//**************************************************************
// int ContainsTheString()
//**************************************************************

int ContainsTheString( std::function<bool( const std::string & )> match, const std::vector<std::string> &strings )
{
    // count if
    return int( std::count_if( strings.begin(), strings.end(), match ));
}

//**************************************************************
// void show() do nothing
//**************************************************************

void show()
{
}

//**************************************************************
// void show() with values
//**************************************************************

template<typename A, typename... Types>
void show( A a, Types... b )
{
    // display
    std::cout << a << "\n";

    // recurse
    show( b... );
}

//**************************************************************
// int main()
//**************************************************************

int main()
{
    // result
    int result = 0;

    // allocate jobs
    auto job        = std::unique_ptr<Job>( new Job       ( 8 ));    //.
    auto programmer = std::unique_ptr<Job>( new Programmer( 9 ));    //.
    auto pilot      = std::unique_ptr<Job>( new Pilot     ( 7 ));    //.

    // vector
    std::vector<std::unique_ptr<Job>> jobs;

    // move into vector
    jobs.emplace_back( std::move( job        ));    //.
    jobs.emplace_back( std::move( programmer ));    //.
    jobs.emplace_back( std::move( pilot      ));    //.

    // for each...
    for ( const auto &j : jobs )
    {
        // show details
        std::cout << j->name() << ", " << j->doWork() << ", Hours: " << j->hoursRequired() << "\n";
    }

    // new line
    std::cout << "\n";

    // init code
    uint32_t httpResponseCode = 501;

    // get in range
    bool inRange = IsInBounds( httpResponseCode, 500, 599 );

    // init string list
    auto theStrings = std::vector<std::string> { "one", "two", "test" };

    // get count
    auto count = ContainsTheString( []( const std::string &tested ) { return tested == "test"; }, theStrings );

    // output
    std::cout << "Matching strings: " << count << "\n\n";

    // show variadic list of stuff
    show( 1, 2, 3, "Four", std::string( "Five" ));

    // return
    return result;
}
