#include "pch.h"
#include "CppUnitTest.h"
#include "../step1.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace step1UnitTests
{
	TEST_CLASS(jobsUnitTests)
	{
	public:
		TEST_METHOD(TestStandardJobGetName)
		{
			auto nameStr = std::string("Jeremias");
			auto j = std::make_unique<Job>(nameStr, "", 0);

			auto name = j->getName();

			Assert::AreEqual(name, nameStr);
		}

		TEST_METHOD(TestStandardJobGetDescription) {
			auto descrStr = std::string("programming in C++");
			auto j = std::make_unique<Job>("", descrStr, 0);

			auto description = j->getDescription();

			Assert::AreEqual(description, descrStr);
		}

		TEST_METHOD(TestStandardJobGetHours) {
			auto h = static_cast<unsigned int>(10);
			auto j = std::make_unique<Job>("", "", h);

			auto hours = j->getHours();

			Assert::AreEqual(hours, h);
		}
		//-----
		TEST_METHOD(TestStandardJobSetGetParent) {
			auto j = std::make_unique<Job>("Child", "", 0);
			auto p = std::make_shared<Job>("Parent", "", 0);

			j->setParent(p);

			auto w = j->getParent().lock();

			Assert::IsNotNull(w.get());
		}

		TEST_METHOD(TestStandardJobIsSupervisor) {
			auto j = std::make_unique<Job>("Job", "description", 0);

			auto i = j->isSupervisor();

			Assert::AreEqual(i, false);
		}

		TEST_METHOD(TestStandardJobAddSupervised) {
			auto j = std::make_unique<Job>("Parent", "", 0);
			auto c = std::make_shared<Job>("Child", "", 0);

			auto i = j->addSupervisedJob(c);

			Assert::AreEqual(i, false);
		}

		TEST_METHOD(TestStandardJobRemoveSupervised) {
			auto j = std::make_unique<Job>("Parent", "", 0);
			auto c = std::make_shared<Job>("Child", "", 0);

			auto i = j->removeSupervisedJob(c);

			Assert::AreEqual(i, false);
		}
		//-----
		TEST_METHOD(TestProgrammerGetName)
		{
			auto nameStr = std::string("Jeremias");
			auto j = std::make_unique<Programmer>(nameStr, "", 0);

			auto name = j->getName();

			Assert::AreEqual(name, nameStr);
		}

		TEST_METHOD(TestProgrammerGetDescription) {
			auto descrStr = std::string("programming in C++");
			auto j = std::make_unique<Programmer>("", descrStr, 0);

			auto description = j->getDescription();

			Assert::AreEqual(description, descrStr);
		}

		TEST_METHOD(TestProgrammerGetHours) {
			auto h = static_cast<unsigned int>(40);
			auto j = std::make_unique<Programmer>("", "", h);

			auto hours = j->getHours();

			Assert::AreEqual(hours, h);
		}
		//-----
		TEST_METHOD(TestProgrammerSetGetParent) {
			auto j = std::make_unique<Programmer>("Child", "", 0);
			auto p = std::make_shared<Programmer>("Parent", "", 0);

			j->setParent(p);

			auto w = j->getParent().lock();

			Assert::IsNotNull(w.get());
		}

		TEST_METHOD(TestProgrammerIsSupervisor) {
			auto j = std::make_unique<Programmer>("Job", "description", 0);

			auto i = j->isSupervisor();

			Assert::AreEqual(i, false);
		}

		TEST_METHOD(TestProgrammerAddSupervised) {
			auto j = std::make_unique<Programmer>("Parent", "", 0);
			auto c = std::make_shared<Job>("Child", "", 0);

			auto i = j->addSupervisedJob(c);

			Assert::AreEqual(i, false);
		}

		TEST_METHOD(TestProgrammerRemoveSupervised) {
			auto j = std::make_unique<Programmer>("Parent", "", 0);
			auto c = std::make_shared<Job>("Child", "", 0);

			auto i = j->removeSupervisedJob(c);

			Assert::AreEqual(i, false);
		}
		//-----
		TEST_METHOD(TestPilotGetName)
		{
			auto nameStr = std::string("Jeremiah");
			auto j = std::make_unique<Pilot>(nameStr, "", 0);

			auto name = j->getName();

			Assert::AreEqual(name, nameStr);
		}

		TEST_METHOD(TestPilotGetDescription) {
			auto descrStr = std::string("flying a Boeing");
			auto j = std::make_unique<Pilot>("", descrStr, 0);

			auto description = j->getDescription();

			Assert::AreEqual(description, descrStr);
		}

		TEST_METHOD(TestPilotGetHours) {
			auto h = static_cast<unsigned int>(12);
			auto j = std::make_unique<Pilot>("", "", h);

			auto hours = j->getHours();

			Assert::AreEqual(hours, h);
		}
		//-----
		TEST_METHOD(TestPilotSetGetParent) {
			auto j = std::make_unique<Pilot>("Child", "", 0);
			auto p = std::make_shared<Pilot>("Parent", "", 0);

			j->setParent(p);

			auto w = j->getParent().lock();

			Assert::IsNotNull(w.get());
		}

		TEST_METHOD(TestPilotIsSupervisor) {
			auto j = std::make_unique<Pilot>("Job", "description", 0);

			auto i = j->isSupervisor();

			Assert::AreEqual(i, false);
		}

		TEST_METHOD(TestPilotAddSupervised) {
			auto j = std::make_unique<Pilot>("Parent", "", 0);
			auto c = std::make_shared<Job>("Child", "", 0);

			auto i = j->addSupervisedJob(c);

			Assert::AreEqual(i, false);
		}

		TEST_METHOD(TestPilotRemoveSupervised) {
			auto j = std::make_unique<Pilot>("Parent", "", 0);
			auto c = std::make_shared<Job>("Child", "", 0);

			auto i = j->removeSupervisedJob(c);

			Assert::AreEqual(i, false);
		}
		//-----
		TEST_METHOD(TestSupervisorGetName)
		{
			auto nameStr = std::string("Josh");
			auto j = std::make_unique<Supervisor>(nameStr, "", 0);

			auto name = j->getName();

			Assert::AreEqual(name, nameStr);
		}

		TEST_METHOD(TestSupervisorGetDescription) {
			auto descrStr = std::string("supervising everyone");
			auto j = std::make_unique<Supervisor>("", descrStr, 0);

			auto description = j->getDescription();

			Assert::AreEqual(description, descrStr);
		}

		TEST_METHOD(TestSupervisorJobGetHours) {
			auto h = static_cast<unsigned int>(60);
			auto j = std::make_unique<Supervisor>("", "", h);

			auto hours = j->getHours();

			Assert::AreEqual(hours, h);
		}
		//-----
		TEST_METHOD(TestSupervisorSetGetParent) {
			auto j = std::make_unique<Supervisor>("Child", "", 0);
			auto p = std::make_shared<Pilot>("Parent", "", 0);

			j->setParent(p);

			auto w = j->getParent().lock();

			Assert::IsNotNull(w.get());
		}

		TEST_METHOD(TestSupervisorIsSupervisor) {
			auto j = std::make_unique<Supervisor>("Job", "description", 0);

			auto i = j->isSupervisor();

			Assert::AreEqual(i, true);
		}

		TEST_METHOD(TestSupervisorAddSupervised) {
			auto j = std::make_shared<Supervisor>("Parent", "", 0);
			auto c = std::make_shared<Job>("Child", "", 0);

			auto i = j->addSupervisedJob(c);

			Assert::AreEqual(i, true);
		}

		TEST_METHOD(TestSupervisorRemoveSupervised) {
			auto j = std::make_shared<Supervisor>("Parent", "", 0);
			auto c = std::make_shared<Job>("Child", "", 0);

			auto i = j->removeSupervisedJob(c);

			Assert::AreEqual(i, true);
		}
		//-----
		TEST_METHOD(TestStandardJobDoWork) {
			auto expected = std::string("My work involves cleaning everything up\n");
			std::unique_ptr<IJob, std::default_delete<IJob>> j = std::make_unique<Job>("", "cleaning everything up", 0);

			std::stringstream buffer;
			std::streambuf* sbuf = std::cout.rdbuf();
			std::cout.rdbuf(buffer.rdbuf());

			j->DoWork();

			std::cout.rdbuf(sbuf);
			std::cout << "std original buffer: \n";
			std::cout << buffer.get();

			Assert::AreEqual(expected, buffer.str());
		}

		TEST_METHOD(TestProgrammerPolymorphicDoWork) {
			auto expected = std::string("I'm a programmer, and... My work involves programming in C++\n");
			std::unique_ptr<IJob, std::default_delete<IJob>> j = std::make_unique<Programmer>("", "programming in C++", 0);

			std::stringstream buffer;
			std::streambuf* sbuf = std::cout.rdbuf();
			std::cout.rdbuf(buffer.rdbuf());

			j->DoWork();

			std::cout.rdbuf(sbuf);
			std::cout << "std original buffer: \n";
			std::cout << buffer.get();

			Assert::AreEqual(expected, buffer.str());
		}

		TEST_METHOD(TestPilotPolymorphicDoWork) {
			auto expected = std::string("I'm a pilot, and... My work involves flying a Boeing\n");
			std::unique_ptr<IJob, std::default_delete<IJob>> j = std::make_unique<Pilot>("", "flying a Boeing", 0);

			std::stringstream buffer;
			std::streambuf* sbuf = std::cout.rdbuf();
			std::cout.rdbuf(buffer.rdbuf());

			j->DoWork();

			std::cout.rdbuf(sbuf);
			std::cout << "std original buffer: \n";
			std::cout << buffer.get();

			Assert::AreEqual(expected, buffer.str());
		}

		TEST_METHOD(TestSupervisorPolymorphicDoWork) {
			std::shared_ptr<IJob> j = std::make_shared<Supervisor>("Supervisor", "supervising everyone", 0);
			auto c = std::make_shared<Job>("Person", "", 0);

			j->addSupervisedJob(c);

			std::stringstream buffer;
			std::streambuf* sbuf = std::cout.rdbuf();
			std::cout.rdbuf(buffer.rdbuf());

			j->DoWork();

			std::cout.rdbuf(sbuf);
			std::cout << "std original buffer: \n";
			std::cout << buffer.get();

			auto expected = std::string("My work involves supervising everyone\nAnd I supervise the following people:\nPerson");

			Assert::AreEqual(expected, buffer.str());
		}
	};
}
