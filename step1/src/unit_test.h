#include <memory>
#include <vector>
#include <algorithm>
#include <functional>

#include "pilot.h"
#include "programmer.h"
#include "variadic_template.h"

#pragma once

template <typename T>
std::string getFunctionName(const T& func)
{
    return typeid(func).name();
}

void runUnitTests();
