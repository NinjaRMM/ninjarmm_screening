#include <step1/utils.hpp>

uint32_t ContainsTheString(const std::function<bool(std::string)>& test_function, const std::vector<std::string> &strings)
{
    uint32_t counter = 0;

    for ( auto &string : strings )
    {
        if (test_function(string))
        {
            counter++;
        }
    }

    return counter;
}
