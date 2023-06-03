#include <fstream>
#include <streambuf>

#include "unit_test.h"

void runTest(std::function<bool()> unitTest, std::string unitTestName)
{
    std::ofstream nullOutput("/dev/null");
    std::streambuf* originalStreamBuffer = std::cout.rdbuf();
    std::cout.rdbuf(nullOutput.rdbuf());

    if (unitTest()) {
        std::cout.rdbuf(originalStreamBuffer);
        std::cout << unitTestName << ": Passed" << std::endl;
    } else {
        std::cout.rdbuf(originalStreamBuffer);
        std::cout << unitTestName << ": Failed" << std::endl;
        exit(1);
    }
}

bool commonJobGetNameWithDefaultConstructorTest()
{
    CommonJob job;
    return job.getName() == "" ? true : false;
}

bool commonJobGetDescriptionWithDefaultConstructorTest()
{
    CommonJob job;
    return job.getDescription() == "" ? true : false;
}

bool commonJobGetHoursRequiredWithDefaultConstructorTest()
{
    CommonJob job;
    return job.getHoursRequired() == 0 ? true : false;
}

bool commonJobDoWorkWithDefaultConstructorTest()
{
    CommonJob job;
    try {
        job.doWork();
        return true;
    } catch (...) {
        return false;
    }
}

bool commonJobGetNameWithParameterizedConstructorTest()
{
    CommonJob job("common", "description", 1);
    return job.getName() == "common" ? true : false;
}

bool commonJobGetDescriptionWithParameterizedConstructorTest()
{
    CommonJob job("common", "description", 1);
    return job.getDescription() == "description" ? true : false;
}

bool commonJobGetHoursRequiredWithParameterizedConstructorTest()
{
    CommonJob job("common", "description", 1);
    return job.getHoursRequired() == 1 ? true : false;
}

bool commonJobDoWorkWithWithParameterizedConstructorTest()
{
    CommonJob job;
    try {
        job.doWork();
        return true;
    } catch (...) {
        return false;
    }
}

bool programmerGetNameWithDefaultConstructorTest()
{
    Programmer job;
    return job.getName() == "Programmer" ? true : false;
}

bool programmerGetDescriptionWithDefaultConstructorTest()
{
    Programmer job;
    return job.getDescription() == "Designing, implementing and testing software projects." ? true : false;
}

bool programmerGetHoursRequiredWithDefaultConstructorTest()
{
    Programmer job;
    return job.getHoursRequired() == 8 ? true : false;
}

bool programmerDoWorkWithDefaultConstructorTest()
{
    Programmer job;
    try {
        job.doWork();
        return true;
    } catch (...) {
        return false;
    }
}

bool pilotGetNameWithDefaultConstructorTest()
{
    Pilot job;
    return job.getName() == "Pilot" ? true : false;
}

bool pilotGetDescriptionWithDefaultConstructorTest()
{
    Pilot job;
    return job.getDescription() == "Driving a plane from takeoff to landing." ? true : false;
}

bool pilotGetHoursRequiredWithDefaultConstructorTest()
{
    Pilot job;
    return job.getHoursRequired() == 12 ? true : false;
}

bool pilotDoWorkWithDefaultConstructorTest()
{
    Pilot job;
    try {
        job.doWork();
        return true;
    } catch (...) {
        return false;
    }
}

bool pilotToStringWithDefaultConstructorTest()
{
    Pilot job;
    return job.toString() == "name: Pilot description: Driving a plane from takeoff to landing. hoursRequired: 12" ? true : false;
}

bool pilotToStringWithParameterizedConstructorTest()
{
    Pilot job("pilot", "drive airplane.", 20);
    std::cout << job.toString() << std::endl;
    return job.toString() == "name: pilot description: drive airplane. hoursRequired: 20" ? true : false;
}

void runCommonJobUnitTests()
{
    runTest(commonJobGetNameWithDefaultConstructorTest, "commonJobGetNameWithDefaultConstructorTest");
    runTest(commonJobGetDescriptionWithDefaultConstructorTest, "commonJobGetDescriptionWithDefaultConstructorTest");
    runTest(commonJobGetHoursRequiredWithDefaultConstructorTest, "commonJobGetHoursRequiredWithDefaultConstructorTest");
    runTest(commonJobDoWorkWithDefaultConstructorTest, "commonJobDoWorkWithDefaultConstructorTest");
    runTest(commonJobGetNameWithParameterizedConstructorTest, "commonJobGetNameWithParameterizedConstructorTest");
    runTest(commonJobGetDescriptionWithParameterizedConstructorTest, "commonJobGetDescriptionWithParameterizedConstructorTest");
    runTest(commonJobGetHoursRequiredWithParameterizedConstructorTest, "commonJobGetHoursRequiredWithParameterizedConstructorTest");
    runTest(commonJobDoWorkWithWithParameterizedConstructorTest, "commonJobDoWorkWithWithParameterizedConstructorTest");
}

void runProgrammerUnitTests()
{
    runTest(programmerGetNameWithDefaultConstructorTest, "programmerGetNameWithDefaultConstructorTest");
    runTest(programmerGetDescriptionWithDefaultConstructorTest, "programmerGetDescriptionWithDefaultConstructorTest");
    runTest(programmerGetHoursRequiredWithDefaultConstructorTest, "programmerGetHoursRequiredWithDefaultConstructorTest");
    runTest(programmerDoWorkWithDefaultConstructorTest, "programmerDoWorkWithDefaultConstructorTest");
}

void runPilotUnitTests()
{
    runTest(pilotGetNameWithDefaultConstructorTest, "pilotGetNameWithDefaultConstructorTest");
    runTest(pilotGetDescriptionWithDefaultConstructorTest, "pilotGetDescriptionWithDefaultConstructorTest");
    runTest(pilotGetHoursRequiredWithDefaultConstructorTest, "pilotGetHoursRequiredWithDefaultConstructorTest");
    runTest(pilotDoWorkWithDefaultConstructorTest, "pilotDoWorkWithDefaultConstructorTest");
    runTest(pilotToStringWithDefaultConstructorTest, "pilotToStringWithDefaultConstructorTest");
    runTest(pilotToStringWithParameterizedConstructorTest, "pilotToStringWithParameterizedConstructorTest");
}

void runUnitTests()
{
    runCommonJobUnitTests();
    runProgrammerUnitTests();
    runPilotUnitTests();
}
