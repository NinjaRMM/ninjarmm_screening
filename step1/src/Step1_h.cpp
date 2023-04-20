/*
* Step1_h.cpp
* This file covers step 'h' of 'Requirements.txt'
* There are unit tests for each type considered.
*/

#include <cstdint>
#include <string>
#include <iostream>
#include <cassert>

template<typename T>
bool IsInBounds(T code, T lowest, T highest)
{
    if (lowest <= code && code <= highest) {
        return true;
    }
    return false;
}

template<typename T>
std::string printResult(T code, T lowest, T highest)
{
    if (IsInBounds<uint32_t>(code, lowest, highest)) {
        return " is in bounds";
    }
    return " not is in bounds";
}

template<typename T>
void test_IsInBounds_onOutOfBounds_ShouldReturnFalse()
{
    T lcode = 1000l, llowest = 1001l, lhighest = 1002l;
    assert(IsInBounds(lcode, llowest, lhighest)==false);
}

void test_IsInBounds_onOutOfBounds_ShouldReturnTrue()
{
    uint32_t httpResponseCode = 500, httpLowest = 500, httpHighest = 599;
    assert(IsInBounds(httpResponseCode, httpLowest, httpHighest) == true);

    char ccode = 't', clowest = 'a', chighest = 'z';
    assert(IsInBounds(ccode, clowest, chighest) == true);
}

int main()
{
    // uint32_t type
    uint32_t httpResponseCode = 500, httpLowest = 500, httpHighest = 599;
    std::cout << httpResponseCode << " " << printResult<uint32_t>(httpResponseCode, httpLowest, httpHighest) << std::endl;

    // long type
    long lcode = 1000, llowest = 1001, lhighest = 1002;
    std::cout << lcode << printResult<long>(lcode, llowest, lhighest) << std::endl;

    // char type
    char ccode = 't', clowest = 'a', chighest = 'z';
    std::cout << ccode << printResult<long>(ccode, clowest, chighest) << std::endl;

    test_IsInBounds_onOutOfBounds_ShouldReturnFalse<long>();
    test_IsInBounds_onOutOfBounds_ShouldReturnTrue();
}