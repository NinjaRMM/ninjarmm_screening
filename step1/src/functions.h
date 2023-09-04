#include <functional>
#include <iostream>

template <typename T>
bool IsInBounds(T value, T lower, T upper)
{
    return lower <= value && value <= upper;
}

void IsInBoundsPrinter(bool result)
{
    if (result)
        std::cout << "Is in bounds" << std::endl;
    else
        std::cout << "Is NOT in bounds" << std::endl;
}

/* bonus (variadic template) */
template <typename T>
void PrintInReverseOrder(const T& t)
{
    std::cout << t << std::endl;
}

template <typename T, typename... Args >
void PrintInReverseOrder(const T& t, Args... moreToPrint)
{
    PrintInReverseOrder(moreToPrint...);

    std::cout << t << std::endl;
}

size_t ContainsTheString(std::function<bool(const std::string &tested)> checker, std::vector<std::string> &strings)
{
    size_t matches = 0U;
    for (const auto &string : strings)
    {
        if (checker(string))
            matches++;
    }
    return matches;
}