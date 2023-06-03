#include <memory>
#include <vector>
#include <algorithm>
#include <functional>

#include "pilot.h"
#include "programmer.h"
#include "variadic_template.h"

#pragma once

void executeAllJobFunctions(std::shared_ptr<Job> job);

void dynamicAllocateEachTypeOfJob();

void storeEachInstanceInACollection();

void iterateTheCollectionAndCallEachAvailableInterfaceFunctions();

template<typename T, typename U>
bool IsInBounds(T value, U lowerBound, U upperBound) {
    using CommonType = typename std::common_type<T, U>::type;
    return (value >= static_cast<CommonType>(lowerBound)) && 
        (value <= static_cast<CommonType>(upperBound));
}

void callTheTemplateFunctionAndPrintOutTheInputsAndResult();

std::uint32_t ContainsTheString(std::function<bool(const std::string&)> test, const std::vector<std::string>& itens);

void callTheContainsTheStringFunctionThenPrintOutTheResult();

void showTheImplementationOfVariadicTemplateWorking();

void executeRequiredTasks();
