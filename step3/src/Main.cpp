// Application Headers
#include "src/asked/Asked.hpp"
#include "src/extra/Extra.hpp"
#include "src/mpl/Monotonic.hpp"
#include "src/mpl/Polymorphic.hpp"
#include "src/oop/Pilot.hpp"
#include "src/oop/Programmer.hpp"

/// Main entry point for the application
auto main() -> int
{
    /// Asked requirements

    job::VecJob js{};

    js.push_back(std::make_unique<job::Pilot>("Alex", "Flying around the world", 10));
    js.push_back(std::make_unique<job::Programmer>("Jane", "Developing Web apps", 24));
    js.push_back(std::make_unique<job::Pilot>("John", "Seeking the skies", 12));
    js.push_back(std::make_unique<job::Programmer>("Terry", "Software engineering", 8));

    util::PrintSeparator();
    util::DisplayJobsInfo(js);

    util::PrintSeparator();
    util::IsInBounds<uint32_t>(400, 500, 599);

    util::PrintSeparator();
    util::IsInBounds<uint32_t>(550, 500, 599);

    util::PrintSeparator();
    auto v = VecStr{"one", "test", "two", "test", "three", "test"};

    auto count = util::ContainsTheString([](const std::string& tested) { return tested == "test"; }, v);
    std::cout << "The number of occurrences of word 'test' is: " << count << std::endl;


    /// Extra features; Showing 'Variadic Templates'

    util::PrintSeparator();

    auto r = extra::MaxNumber(4,6,1,3,8,9,0,2,5,7);
    std::cout << "The greater number is: " << r << std::endl;

    util::PrintSeparator();
    std::cout << "Left Fold - Binary Op Sum" << std::endl;
    std::cout << "SUM is: " << extra::LeftFoldBinaryOpSum(4,6,1,3,8,9,0,2,5,7) << std::endl;


    /// Extra feature; Attempts to show 'Parametric Polymorphism' via 'Concepts'

    util::PrintSeparator();

    mpl::TypesT p1{mpl::PilotT{"Biden", "Flying plane #1", 1}};
    mpl::TypesT p2{mpl::PilotT{"Tommy", "Flying plane #2", 2}};

    mpl::TypesT p3{mpl::ProgrammerT{"Wick", "Writing code #1", 3}};
    mpl::TypesT p4{mpl::ProgrammerT{"Mary", "Writing code #2", 4}};

    mpl::VectorT vec{p1, p2, p3, p4};
    mpl::DisplayJobs(vec);


    /// Extra feature; Showing 'PMR' allocation and 'Monotonic' memory resource

    util::PrintSeparator();

    pmr::Allocation();

    return 0;
}