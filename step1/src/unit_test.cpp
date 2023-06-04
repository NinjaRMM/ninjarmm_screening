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

bool programmerGetNameWithParameterizedConstructorTest()
{
    Programmer job("common", "description", 1);
    return job.getName() == "common" ? true : false;
}

bool programmerGetDescriptionWithParameterizedConstructorTest()
{
    Programmer job("common", "description", 1);
    return job.getDescription() == "description" ? true : false;
}

bool programmerGetHoursRequiredWithParameterizedConstructorTest()
{
    Programmer job("common", "description", 1);
    return job.getHoursRequired() == 1 ? true : false;
}

bool programmerDoWorkWithParameterizedConstructorTest()
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

bool pilotGetNameWithParameterizedConstructorTest()
{
    Pilot job("common", "description", 1);
    return job.getName() == "common" ? true : false;
}

bool pilotGetDescriptionWithParameterizedConstructorTest()
{
    Pilot job("common", "description", 1);
    return job.getDescription() == "description" ? true : false;
}

bool pilotGetHoursRequiredWithParameterizedConstructorTest()
{
    Pilot job("common", "description", 1);
    return job.getHoursRequired() == 1 ? true : false;
}

bool pilotDoWorkWithParameterizedConstructorTest()
{
    Pilot job;
    try {
        job.doWork();
        return true;
    } catch (...) {
        return false;
    }
}

bool pilotToStringWithParameterizedConstructorTest()
{
    Pilot job("pilot", "drive airplane.", 20);
    std::cout << job.toString() << std::endl;
    return job.toString() == "name: pilot description: drive airplane. hoursRequired: 20" ? true : false;
}

void runProgrammerUnitTests()
{
    runTest(programmerGetNameWithDefaultConstructorTest, "programmerGetNameWithDefaultConstructorTest");
    runTest(programmerGetDescriptionWithDefaultConstructorTest, "programmerGetDescriptionWithDefaultConstructorTest");
    runTest(programmerGetHoursRequiredWithDefaultConstructorTest, "programmerGetHoursRequiredWithDefaultConstructorTest");
    runTest(programmerDoWorkWithDefaultConstructorTest, "programmerDoWorkWithDefaultConstructorTest");
    runTest(programmerGetNameWithParameterizedConstructorTest, "programmerGetNameWithParameterizedConstructorTest");
    runTest(programmerGetDescriptionWithParameterizedConstructorTest, "programmerGetDescriptionWithParameterizedConstructorTest");
    runTest(programmerGetHoursRequiredWithParameterizedConstructorTest, "programmerGetHoursRequiredWithParameterizedConstructorTest");
    runTest(programmerDoWorkWithParameterizedConstructorTest, "programmerDoWorkWithParameterizedConstructorTest");
}

void runPilotUnitTests()
{
    runTest(pilotGetNameWithDefaultConstructorTest, "pilotGetNameWithDefaultConstructorTest");
    runTest(pilotGetDescriptionWithDefaultConstructorTest, "pilotGetDescriptionWithDefaultConstructorTest");
    runTest(pilotGetHoursRequiredWithDefaultConstructorTest, "pilotGetHoursRequiredWithDefaultConstructorTest");
    runTest(pilotDoWorkWithDefaultConstructorTest, "pilotDoWorkWithDefaultConstructorTest");
    runTest(pilotToStringWithDefaultConstructorTest, "pilotToStringWithDefaultConstructorTest");
    runTest(pilotGetNameWithParameterizedConstructorTest, "pilotGetNameWithParameterizedConstructorTest");
    runTest(pilotGetDescriptionWithParameterizedConstructorTest, "pilotGetDescriptionWithParameterizedConstructorTest");
    runTest(pilotGetHoursRequiredWithParameterizedConstructorTest, "pilotGetHoursRequiredWithParameterizedConstructorTest");
    runTest(pilotDoWorkWithParameterizedConstructorTest, "pilotDoWorkWithParameterizedConstructorTest");
    runTest(pilotToStringWithParameterizedConstructorTest, "pilotToStringWithParameterizedConstructorTest");
}

void runUnitTests()
{
    runProgrammerUnitTests();
    runPilotUnitTests();
}
