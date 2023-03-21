#include <iostream>
#include <list>
#include <vector>
#include <functional>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

//
// Function Templates
//
void jobsFunction(void);
void responseCodeFunction(uint32_t, uint32_t, uint32_t);
void stringsFunction(std::vector<std::string>, std::string);

//
// Classes
//
class Job
{
public:
    virtual std::string getName() const
    {
        return "Job";
    }

    virtual std::string getDescription() const
    {
        return "Description";
    }

    virtual void DoWork() const
    {
        std::cout << "My work involves " << getDescription() << std::endl;
    };

    virtual uint getHours() const
    {
        return 0;
    }

    virtual ~Job(){};
};

class Programmer : public Job
{
public:
    std::string getName() const override
    {
        return "Programmer";
    }

    std::string getDescription() const override
    {
        return "writing code";
    }

    uint getHours() const override
    {
        return 44;
    }
};

class Pilot : public Job
{
public:
    std::string getName() const override
    {
        return "Pilot";
    }

    std::string getDescription() const override
    {
        return "driving fast cars";
    }

    uint getHours() const override
    {
        return 60;
    }
};

class SuperWorker : public Job
{
public:
    std::string getName() const override
    {
        return "SuperWorker";
    }

    std::string getDescription() const override
    {
        return "to never stop working";
    }

    uint getHours() const override
    {
        return 120;
    }
};

//
// Templates
//
template <typename T>
bool IsInBounds(const T &value, const T &lower, const T &upper)
{
    return value >= lower && value <= upper;
}

bool operator>(const Job &j1, const Job &j2)
{
    return j1.getHours() >= j2.getHours();
}

template <typename T>
T *greaterThan(T *a, T *b)
{
    if (a > b)
        return a;
    else
        return b;
}

template <typename T, typename... Types>
T *greaterThan(T *a, T *b, Types... args)
{
    return greaterThan(greaterThan(a, b), args...);
}

//
// Testing Class Setup
//
class TestStep1 : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestStep1);
    CPPUNIT_TEST(testProgrammer);
    CPPUNIT_TEST(testPilot);
    CPPUNIT_TEST(testSuperWorker);
    CPPUNIT_TEST(testIsInBounds);
    CPPUNIT_TEST(testVariadic);
    CPPUNIT_TEST(testContainsString);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void) override;
    void tearDown(void) override;

protected:
    void testProgrammer(void);
    void testPilot(void);
    void testSuperWorker(void);
    void testIsInBounds(void);
    void testVariadic(void);
    void testContainsString(void);

private:
    Programmer *pProgrammer;
    Pilot *pPilot;
    SuperWorker *pSuperWorker;
};

size_t ContainsTheString(std::function<bool(const std::string &)> predicate, const std::vector<std::string> &strings)
{
    size_t count = 0;
    for (const auto &str : strings)
    {
        if (predicate(str))
        {
            count++;
        }
    }
    return count;
}

CPPUNIT_TEST_SUITE_REGISTRATION(TestStep1);

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        if (std::string(argv[1]) == "test")
        {
            CPPUNIT_NS::TestResult testresult;

            CPPUNIT_NS::TestResultCollector collectedresults;
            testresult.addListener(&collectedresults);

            CPPUNIT_NS::BriefTestProgressListener progress;
            testresult.addListener(&progress);

            CPPUNIT_NS::TestRunner testrunner;
            testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
            testrunner.run(testresult);

            return collectedresults.wasSuccessful() ? 0 : 1;
        }
        else
        {
            std::cout << "Invalid argument!" << std::endl;
            return EINVAL;
        }
    }
    jobsFunction();

    std::cout << "--------------------------------------" << std::endl;

    responseCodeFunction(503, 500, 599);

    std::cout << "--------------------------------------" << std::endl;

    auto theStrings = std::vector<std::string>{"one", "two", "test"};
    std::string testString = "test";
    stringsFunction(theStrings, testString);

    return 0;
}

void jobsFunction()
{
    Pilot *pPilot = new Pilot();
    Programmer *pProgrammer = new Programmer();

    std::list<Job *> lpJobs;
    lpJobs.push_back(pProgrammer);
    lpJobs.push_back(pPilot);

    for (const auto &pJob : lpJobs)
    {
        std::cout << pJob->getName() << std::endl;
        std::cout << pJob->getDescription() << std::endl;
        std::cout << pJob->getHours() << std::endl;
        pJob->DoWork();
        std::cout << "----------" << std::endl;
    }

    // Variadic function
    SuperWorker *pSuperWorker = new SuperWorker();
    auto pHardestWorking = greaterThan<Job>((Job *)pPilot, (Job *)pProgrammer, (Job *)pSuperWorker);
    std::cout << "The one that works the most is the " << pHardestWorking->getName() << std::endl;
    delete pSuperWorker;

    for (const auto &pJob : lpJobs)
    {
        delete pJob;
    }
}

void responseCodeFunction(uint32_t uiHttpResponseCode, uint32_t uiLowerBound, uint32_t uiUpperBound)
{
    bool isInBounds = IsInBounds<uint32_t>(uiHttpResponseCode, uiLowerBound, uiUpperBound);
    std::cout << "httpResponseCode: " << uiHttpResponseCode << std::endl;
    std::cout << "Is within bounds (" << uiLowerBound << "-" << uiUpperBound << "): " << std::boolalpha << isInBounds << std::endl;
}

void stringsFunction(std::vector<std::string> svList, std::string sCompare)
{
    size_t count = ContainsTheString([sCompare](const std::string &tested)
                                     { return tested == sCompare; },
                                     svList);
    std::cout << "Final count: " << count << std::endl;
}

void TestStep1::setUp(void)
{
    pProgrammer = new Programmer();
    pPilot = new Pilot();
    pSuperWorker = new SuperWorker();
}

void TestStep1::tearDown(void)
{
    delete pProgrammer;
    delete pPilot;
    delete pSuperWorker;
}

void TestStep1::testProgrammer(void)
{
    CPPUNIT_ASSERT(pProgrammer->getName() == "Programmer");
    CPPUNIT_ASSERT(pProgrammer->getDescription() == "writing code");
    CPPUNIT_ASSERT(pProgrammer->getHours() == 44);
}

void TestStep1::testPilot(void)
{
    CPPUNIT_ASSERT(pPilot->getName() == "Pilot");
    CPPUNIT_ASSERT(pPilot->getDescription() == "driving fast cars");
    CPPUNIT_ASSERT(pPilot->getHours() == 60);
}

void TestStep1::testSuperWorker(void)
{
    CPPUNIT_ASSERT(pSuperWorker->getName() == "SuperWorker");
    CPPUNIT_ASSERT(pSuperWorker->getDescription() == "to never stop working");
    CPPUNIT_ASSERT(pSuperWorker->getHours() == 120);
}

void TestStep1::testIsInBounds(void)
{
    CPPUNIT_ASSERT(IsInBounds(503, 500, 600));
    // Test lower bound
    CPPUNIT_ASSERT(IsInBounds(500, 500, 600));
    CPPUNIT_ASSERT(!IsInBounds(499, 500, 600));
    // Test upper bound
    CPPUNIT_ASSERT(IsInBounds(600, 500, 600));
    CPPUNIT_ASSERT(!IsInBounds(601, 500, 600));
}

void TestStep1::testVariadic(void)
{
    auto pHigherHours = greaterThan<Job>((Job *)pPilot, (Job *)pProgrammer, (Job *)pSuperWorker);
    CPPUNIT_ASSERT(pHigherHours->getName() == "SuperWorker");

    pHigherHours = greaterThan<Job>((Job *)pPilot, (Job *)pProgrammer);
    CPPUNIT_ASSERT(pHigherHours->getName() == "Pilot");
}

void TestStep1::testContainsString(void)
{
    std::vector<std::string> theStrings = std::vector<std::string>{"test", "test", "test"};
    std::string testString = "test";
    size_t count = ContainsTheString([testString](const std::string &tested)
                                     { return tested == testString; },
                                     theStrings);
    CPPUNIT_ASSERT(count == 3);
    testString = "nontest";
    count = ContainsTheString([testString](const std::string &tested)
                              { return tested == testString; },
                              theStrings);
    CPPUNIT_ASSERT(count == 0);
    theStrings.push_back("test2");
    theStrings.push_back("test2");
    theStrings.push_back("test2");
    theStrings.push_back("test2");
    testString = "test2";
    count = ContainsTheString([testString](const std::string &tested)
                              { return tested == testString; },
                              theStrings);
    CPPUNIT_ASSERT(count == 4);
    testString = "test";
    count = ContainsTheString([testString](const std::string &tested)
                              { return tested == testString; },
                              theStrings);
    CPPUNIT_ASSERT(count == 3);

    theStrings.clear();
}
