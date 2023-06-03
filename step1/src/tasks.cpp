#include "tasks.h"

void executeAllJobFunctions(std::shared_ptr<Job> job)
{
    job->getName();
    job->getDescription();
    job->getHoursRequired();
    job->doWork();
}

void dynamicAllocateEachTypeOfJob()
{
    std::shared_ptr<Job> programerPtr, pilotPtr;
    programerPtr = std::make_unique<Programmer>();
    pilotPtr = std::make_unique<Pilot>();
}

void storeEachInstanceInACollection()
{
    std::shared_ptr<Job> programerPtr, pilotPtr;
    programerPtr = std::make_unique<Programmer>();
    pilotPtr = std::make_unique<Pilot>();

    std::vector<std::shared_ptr<Job>> jobVector;
    jobVector.push_back(programerPtr);
    jobVector.push_back(pilotPtr);
}

void iterateTheCollectionAndCallEachAvailableInterfaceFunctions()
{
    std::shared_ptr<Job> programerPtr, pilotPtr;
    programerPtr = std::make_unique<Programmer>();
    pilotPtr = std::make_unique<Pilot>();

    std::vector<std::shared_ptr<Job>> jobVector;
    jobVector.push_back(programerPtr);
    jobVector.push_back(pilotPtr);

    std::for_each(jobVector.begin(), jobVector.end(), [](std::shared_ptr<Job> job) {
        executeAllJobFunctions(job);
    });
}

void callTheTemplateFunctionAndPrintOutTheInputsAndResult()
{
    std::uint32_t httpResponseCodeBadGateway = 502;
    std::uint32_t httpResponseCodeOk = 200;

    bool resulTrue = IsInBounds(httpResponseCodeBadGateway, 500, 599);
    std::cout << "Input httpResponseCodeBadGateway: " << httpResponseCodeBadGateway << std::endl;
    std::cout << "Input Lower Bound: " << 500 << std::endl;
    std::cout << "Input Upper Bound: " << 599 << std::endl;
    std::string resultTrueStr = resulTrue ? "true" : "false";
    std::cout << "Result: " << resultTrueStr << std::endl;

    bool resultFalse = IsInBounds(httpResponseCodeOk, 500, 599);
    std::cout << "Input httpResponseCodeOk: " << httpResponseCodeOk << std::endl;
    std::cout << "Input Lower Bound: " << 500 << std::endl;
    std::cout << "Input Upper Bound: " << 599 << std::endl;
    std::string resultFalseStr = resultFalse ? "true" : "false";
    std::cout << "Result: " << resultFalseStr << std::endl;
}

std::uint32_t ContainsTheString(std::function<bool(const std::string&)> test, const std::vector<std::string>& itens)
{
    std::uint32_t matches = 0;
    std::for_each(itens.begin(), itens.end(), [&test, &matches](std::string item) {
        if (test(item))
            matches++;
    });
    return matches;
}

void callTheContainsTheStringFunctionThenPrintOutTheResult()
{
    auto theStrings = std::vector<std::string> { "one", "two", "test"};
	auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    std::string matchesStr = count == 1 ? "match" : "matches";  
    std::cout << "The string has " << count << " " << matchesStr <<" of the string \"test\"" << std::endl;
}

void showTheImplementationOfVariadicTemplateWorking()
{
    //variadic template + SFINAE technique + recursion
    std::uint16_t uint16tVar = 120;
    std::uint32_t uint32tVar = 1234567890;
    double doubleVar = 8.7;
    char charVar = 'a';
    std::string stringVar = "abcdefgh";
    Programmer programmer;
    Pilot pilot;
    std::vector<std::string> result;

    convertToStringVector(result, uint16tVar, uint32tVar, doubleVar, charVar, stringVar, programmer, pilot);

    std::cout << "The generated array of the variadic template sample is: [";

    for (auto it = result.begin(); it != result.end(); ++it) {
        if (it == std::prev(result.end())) {
            std::cout << *it;
        } else {
            std::cout << *it << ", ";
        }
    }

    std::cout << "]" << std::endl;
}

void executeRequiredTasks()
{
    dynamicAllocateEachTypeOfJob();
    storeEachInstanceInACollection();
    iterateTheCollectionAndCallEachAvailableInterfaceFunctions();
    callTheTemplateFunctionAndPrintOutTheInputsAndResult();
    callTheContainsTheStringFunctionThenPrintOutTheResult();
    showTheImplementationOfVariadicTemplateWorking();
}
