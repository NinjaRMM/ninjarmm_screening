#include "pch.h"
#include "CppUnitTest.h"
#include "../Step3B/Step3B.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest2
{
	TEST_CLASS(UnitTest2)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			std::string test_name = "Joe";
			Programmer testProgrammer("Joe", "Programmer", 8);
			std::string job_name = testProgrammer.GetName();
			Assert::AreEqual(job_name, test_name);
		}
		TEST_METHOD(TestMethod2)
		{
			std::string test_name = "Milo";
			Pilot testPilot("Milo", "Pilot", 8);
			std::string job_name = testPilot.GetName();
			Assert::AreEqual(job_name, test_name);
		}
		TEST_METHOD(TestMethod3)
		{
			std::string test_descr = "Programmer";
			Programmer testProgrammer("Joe", "Programmer", 8);
			std::string job_desc = testProgrammer.GetDescription();
			Assert::AreEqual(job_desc, test_descr);
		}
		TEST_METHOD(TestMethod4)
		{
			std::string test_descr = "Pilot";
			Pilot testPilot("Milo", "Pilot", 8);
			std::string job_desc = testPilot.GetDescription();
			Assert::AreEqual(job_desc, test_descr);
		}
		TEST_METHOD(TestMethod5)
		{
			Programmer testProgrammer("Joe", "Programmer", 8);
			int testNumHours = testProgrammer.GetNumReqHours();
			Assert::AreEqual(testNumHours, 8);
		}
		TEST_METHOD(TestMethod6)
		{
			Pilot testPilot("Milo", "Pilot", 15);
			int testNumHours = testPilot.GetNumReqHours();
			Assert::AreEqual(testNumHours, 15);
		}
		TEST_METHOD(TestMethod7)
		{
			bool is_in_bounds = IsInBounds<uint32_t>(501, 500, 599);
			Assert::AreEqual(is_in_bounds, true);
		}
		TEST_METHOD(TestMethod8)
		{
			bool is_in_bounds = IsInBounds<uint32_t>(499, 500, 599);
			Assert::AreEqual(is_in_bounds, false);
		}
		TEST_METHOD(TestMethod9)
		{
			bool is_in_bounds = IsInBounds<uint32_t>(709, 500, 599);
			Assert::AreEqual(is_in_bounds, false);
		}
		TEST_METHOD(TestMethod10)
		{
			bool is_in_bounds = IsInBounds<uint32_t>(-1, 500, 599);
			Assert::AreEqual(is_in_bounds, false);
		}
		TEST_METHOD(TestMethod11)
		{
			bool is_in_bounds = IsInBounds<uint32_t>(500, 500, 599);
			Assert::AreEqual(is_in_bounds, true);
		}
		TEST_METHOD(TestMethod12)
		{
			bool is_in_bounds = IsInBounds<uint32_t>(599, 500, 599);
			Assert::AreEqual(is_in_bounds, true);
		}
		TEST_METHOD(TestMethod13)
		{
			std::vector<std::string> theStrings;
			theStrings.push_back("one");
			theStrings.push_back("two");
			theStrings.push_back("test");
			int contains_string_count = ContainsTheString([](const std::string& tested) { std::string test_string = "test"; return (tested == test_string); }, theStrings);
			Assert::AreEqual(contains_string_count, 1);
		}
		TEST_METHOD(TestMethod14)
		{
			std::vector<std::string> theStrings;
			theStrings.push_back("one");
			theStrings.push_back("two");
			theStrings.push_back("test");
			theStrings.push_back("test");
			int contains_string_count = ContainsTheString([](const std::string& tested) { std::string test_string = "test"; return (tested == test_string); }, theStrings);
			Assert::AreEqual(contains_string_count, 2);
		}
	};
}
