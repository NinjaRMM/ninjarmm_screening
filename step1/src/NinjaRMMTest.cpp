/*
* This program uses C++/14. It was developed on Windows (via Visual C++) and Linux (via https://www.onlinegdb.comm).
*/ 

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include <cstdlib>


// Identify the platform.
// Only Windows and Linux are supported; append new ones as needed.
#if defined(_WIN32) || defined(_WIN64)
	#define WINDOWS_OS

	// Extra setup for memory leak detection.
	#if defined(_DEBUG)
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>
		#include <string.h>	// for _stricmp
		#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
	#endif
#else
	#define LINUX_OS
	#include <strings.h>	// for strcasecmp
#endif

/*
* Unit Test Macros
*/
#define TEST_TRUE(CONDITION) if (!(CONDITION)) std::cout << "Failed: " #CONDITION << std::endl;


/*
* StrUtil
*
* String manipulation utilities
*/
class StrUtil
{
public:
	/*
	* String-to-int conversions
	*/
	static int ToInt(const char* str)
	{
		return atoi(str);
	}
	static int ToInt(const std::string& str)
	{
		return ToInt(str.c_str());
	}

	/*
	* Caseless comparison
	*/
	static int CompareNoCase(const char* left, const char* right)
	{
		// Encapsulate supported platform (Windows/Linux) here so that
		// application code doesn't get littered with compiler directives.
#if defined(WINDOWS_OS)
		return _stricmp(left, right);
#else
		return strcasecmp(left, right);
#endif
	}
	
	struct NoCaseCompare // Utility struct used by map<string> instances for caseless comparison.
	{
		bool operator () (const std::string& left, const std::string& right) const
		{
			return CompareNoCase(left.c_str(), right.c_str()) < 0;
		}
	};

	/*
	* Define a C++ rendition of sprintf, mainly to avoid explicily calling string::c_str(),
	* but also helpful for classes that require specialized string conversion. (See ancillary functions.)
	*/
	template <typename ...Arg>
	static void Print(std::ostream& out, const char* format, const Arg& ...args)
	{
		size_t length = snprintf(nullptr, 0, format, FormatPrintArg(args) ...);
		if (length++)
		{
			std::vector<char> buffer(length); // use a char vector for auto-cleanup.

			snprintf(buffer.data(), length, format, FormatPrintArg(args) ...);

			out << buffer.data() << std::endl;
		}
	}

private:
	/*
	* Ancillary functions used by Print().
	* Additional implementations can be defined that would convert a complex type (i.e., class)
	* to a primitive type that is supported by sprintf , e.g. int, double, const char*.
	*/
	template <typename T>
	static T FormatPrintArg(T value) // covers all primitive types
	{
		return value;
	}
	static const char* FormatPrintArg(const std::string& value) // specialization for std::string
	{
		return value.data();
	}
};


/*
* Job
* 
* Abstract class to define basic interface for all Job types.
* All concrete classes must define GetName, GetDescription, and GetRequiredHours methods.
*/
class Job
{
public:
	using Ptr = std::shared_ptr<Job>;
	using Vector = std::vector<Ptr>;

	// Default constructor/destructor; they do nothing, but can be useful for setting breakpoints.
	Job()
	{
	}
	virtual ~Job()
	{
	}

	/* Basic properties - overrides required */
	virtual const std::string& GetName() const = 0;
	virtual const std::string& GetDescription() const = 0;
	virtual double GetRequiredHours() const = 0;

	/* Operations */
	virtual void DoWork()
	{
		std::cout << "My work involves " << GetDescription() << std::endl;
	}
};

/*
* StandardJob
* 
* Defines a base class that implements basic properties and required overrides.
* Useful for concrete classes that don't want to manage data members or define overrides.
*/
class StandardJob : public Job
{
public:
	StandardJob(const StandardJob& origin) :
		m_name(origin.m_name),
		m_description(origin.m_description),
		m_requiredHours(origin.m_requiredHours)
	{
	}
	StandardJob(StandardJob&& origin) :
		m_name(std::move(origin.m_name)),
		m_description(std::move(origin.m_description)),
		m_requiredHours(std::exchange(origin.m_requiredHours, 0.0))
	{
	}
	StandardJob(double requiredHours, const std::string& name, const std::string& description) :
		m_name(name),
		m_description(description),
		m_requiredHours(requiredHours)
	{
	}

	virtual const std::string& GetName() const
	{
		return m_name;
	}
	virtual const std::string& GetDescription() const
	{
		return m_description;
	}
	virtual double GetRequiredHours() const
	{
		return m_requiredHours;
	}

protected:
	std::string m_name;
	std::string m_description;
	double m_requiredHours;
};

/*
* Programmer
* 
* Derived directly from Job; must define all required overrides.
* For this case, we just use static data, but the implementation is also 
* suitable for cases in which the data members are maintained by a different
* (possibly dynamically allocated) underlying structure than StandardJob.
*/
class Programmer : public Job
{
public:
	virtual const std::string& GetName() const
	{
		static const std::string NAME = "programmer";
		return NAME;
	}
	virtual const std::string& GetDescription() const
	{
		static const std::string DESCRIPTION = "writing programs";
		return DESCRIPTION;
	}
	virtual double GetRequiredHours() const
	{
		return 60;
	}
};

/*
* Pilot
* 
* Derived from StandardJob; no need to maintain data members.
*/
class Pilot : public StandardJob
{
public:
	Pilot(const Pilot& origin) :
		StandardJob(origin)
	{
	}
	Pilot(Pilot&& origin) :
		StandardJob(std::move(origin))
	{
	}
	Pilot(double requiredHours) :
		StandardJob(0, "pilot", "flying planes")
	{
		SetRequiredHours(requiredHours);
	}

	void SetRequiredHours(double requiredHours)
	{
		m_requiredHours = std::min(80.0, requiredHours);
	}
};


/*
* Test functions for specs (h) and (i)
*/
template <typename T> bool IsInBounds(T httpResponseCode, T lowBound, T highBound)
{
	return (httpResponseCode >= lowBound)&&  (httpResponseCode <= highBound);
}

int ContainsTheString(std::function<bool(const std::string&)>&& testFunction, std::vector<std::string>& theStrings)
{
	int count = 0;

	for (const auto& current : theStrings)
	{
		if (testFunction(current))
		{
			count++;
		}
	}

	return count;
}


/*
* NinjaRMMTestApp
* 
* Implements an interactive command line where the user can run a specific test or all of them.
*/
class NinjaRMMTestApp
{
	// Declare the command handler prototype.
	using HandlerFunc = void (NinjaRMMTestApp::*)();	// pointer to method member

public:
	NinjaRMMTestApp() :
		m_exit(false)
	{
		// Initialize the command handler map, which replaces a potentially long cascading if-else-statement in Run(), below.
		// Basically, instead of adding a new if-statement, new commands would be appended to the following initializer list.
		m_commandHandlers = {
			{ "jobs", &NinjaRMMTestApp::RunJobsTest },
			{ "bounds", &NinjaRMMTestApp::RunBoundsTest },
			{ "vector", &NinjaRMMTestApp::RunVectorTest },
			{ "all", &NinjaRMMTestApp::RunAllTests },
			{ "unit", &NinjaRMMTestApp::RunUnitTests },
			{ "clear", &NinjaRMMTestApp::RunClear },
			{ "exit", &NinjaRMMTestApp::RunExit }
		};
	}

	int Run()
	{
		do 
		{
			// Print the prompt.
			std::cout << "Enter a command ('jobs', 'bounds', 'vector', 'all', 'unit', 'clear', 'exit'): ";

			std::string command;
			std::cin >> command;

			// Lookup the handler ...
			auto it = m_commandHandlers.find(command);
			if (it != m_commandHandlers.end())
			{
				// ... execute if found.
				(this->*(it->second))();
			}
			else
			{
				// ... else, display an error.
				std::cerr << "Unknown command '" << command << "'" << std::endl;
			}

			PrintEOL();

		} while (!m_exit);

		return 0;
	}

private:
	/*
	* Command Handlers
	*/
	void RunJobsTest()
	{
		PrintTestTitle("Job Test");

		Job::Vector jobs;
		jobs.push_back(std::make_shared<Programmer>());
		jobs.push_back(std::make_shared<Pilot>(55.555));

		for (const auto& current : jobs)
		{
			std::cout << "I work as a " << current->GetName() << std::endl;

			current->DoWork();

			StrUtil::Print(std::cout, "A %s job requires %0.2f hours", current->GetName(), current->GetRequiredHours());

			PrintEOL();
		}
	}

	void RunBoundsTest()
	{
		RunBoundsTest(0);
	}
	void RunBoundsTest(uint32_t httpResponse)
	{
		PrintTestTitle("Bounds Test");

		if (!httpResponse)
		{
			std::cout << "Enter an HTTP response code: ";

			std::string input;
			std::cin >> input;

			httpResponse = StrUtil::ToInt(input);
		}

		std::string state = IsInBounds<uint32_t>(httpResponse, 500, 599) ? " " : " not ";
		StrUtil::Print(std::cout, "HttpResponse '%d' is%sin bounds.", httpResponse, state);
	}

	void RunVectorTest()
	{
		PrintTestTitle("Vector Test");

		auto theStrings = std::vector<std::string>{ "one", "two", "test" };
		auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);

		std::cout << "Number of matching string(s): " << count << std::endl;
	}

	void RunAllTests()
	{
		RunJobsTest();
		RunBoundsTest(504);	// use non-handler overload
		RunVectorTest();
	}

	void RunUnitTests()
	{
		PrintTestTitle("Unit Test");

		{
			Programmer programmer;
			TEST_TRUE(programmer.GetName() == "programmer");
			TEST_TRUE(programmer.GetDescription() == "writing programs");
			TEST_TRUE(programmer.GetRequiredHours() == 60);
		}
		{
			static int REQUIRED_HOURS = 100;

			Pilot pilot(REQUIRED_HOURS);
			TEST_TRUE(pilot.GetName() == "Pilot");	// Should fail
			TEST_TRUE(pilot.GetName() == "programmer");	// Should fail
			TEST_TRUE(pilot.GetDescription() == "flying planes");
			TEST_TRUE(pilot.GetRequiredHours() == REQUIRED_HOURS);	// Should fail because of 80 hour cap
		}
		{
			auto theStrings = std::vector<std::string>{ "one", "two", "test" };
			TEST_TRUE(ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings) == 1);
			TEST_TRUE(ContainsTheString([](const std::string& tested) { return tested == "hello"; }, theStrings) == 1);
		}
		{
			TEST_TRUE(IsInBounds<uint32_t>(400, 500, 599));
			TEST_TRUE(IsInBounds<uint32_t>(501, 500, 599));
		}
	}

	void RunClear()
	{
#if defined(WINDOWS_OS)
		std::system("cls");
#else
		std::system("clear");
#endif
	}

	void RunExit()
	{
		std::cout << std::endl << "Good-bye";

		m_exit = true;
	}

private:
	/*
	* Print Utilities
	*/
	void PrintTestTitle(const char* title)
	{
		StrUtil::Print(std::cout, "\n*** Running \"%s\" ***\n", title);
	}
	void PrintEOL()
	{
		std::cout << std::endl;
	}

private:
	bool m_exit;
	std::map<std::string, HandlerFunc, StrUtil::NoCaseCompare> m_commandHandlers;
};


/*
* main function
*/
int main()
{
#if defined(WINDOWS_OS) && defined(_DEBUG) // Enable leak detection (Windows only)
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	NinjaRMMTestApp theApp;
	return theApp.Run();
}
