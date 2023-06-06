#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>
#include <functional>

class Job
{
public:
	Job(const std::string& name, const std::string& description, std::uint32_t hoursRequired)
		: m_name(name), m_description(description), m_hoursRequired(hoursRequired)
	{
	}

	virtual std::string GetJobName() const
	{
		return m_name;
	}

	virtual std::string GetJobDescription() const
	{
		return m_description;
	}

	virtual std::uint32_t GetJobRequiredHours() const
	{
		return m_hoursRequired;
	}
	virtual void DoWork() const
	{
		std::cout << "My work involves " + m_description << std::endl;
	}

protected:
	std::string m_name;
	std::string m_description;
	std::uint32_t m_hoursRequired;
};

class Programmer : public Job
{
public:
	Programmer() :
		Job("Programmer", "develop awesome software to help peaple and companies.", 40)
	{
	}

	Programmer(const std::string& name, const std::string& description, std::uint32_t hoursRequired) :
		Job(name, description, hoursRequired)
	{
	}
};

class Pilot : public Job
{
public:
	Pilot() :
		Job("Pilot", "operate and control aircraft along planned routes and during takeoffs and landings", 40)
	{
	}

	Pilot(const std::string& name, const std::string& description, std::uint32_t hoursRequired) :
		Job(name, description, hoursRequired)
	{
	}
};

template <typename T>
bool IsInBounds(const T& value, const T& min, const T& max)
{
	bool result = false;
	if constexpr (std::is_same_v<decltype(std::cout << value), std::ostream&>
		&& std::is_same_v<decltype(value >= min), bool>
		&& std::is_same_v<decltype(value <= max), bool>)
	{
		std::cout << "* Check if value [" << value << "] bounds between min [" << min << "] and max [" << max << "] values..." << std::endl;

		bool result = (value >= min && value <= max);
		std::cout << "    Result: " << std::boolalpha << result << std::endl;
		std::cout << std::endl;
	}

	return result;
}

size_t ContainsTheString(std::function<bool(const std::string&)> testFunction, std::vector<std::string> theStrings)
{
	size_t counter = 0;
	for (const auto& str : theStrings)
	{
		if (testFunction(str))
		{
			counter++;
		}
	}

	return counter;
}

std::string StringConcat()
{
	return "";
}

template <typename T, typename... Args>
std::string StringConcat(const T& head, const Args&... tail)
{
	std::string result;

	if constexpr (std::is_same_v<T, std::string>)
	{
		result += head;
	}
	else if constexpr (std::is_convertible_v<T, std::string>)
	{
		result += std::string(head);
	}
	else if constexpr (std::is_same_v<T, char>)
	{
		result += std::string(1, head);
	}
	else if constexpr (std::is_arithmetic_v<T>)
	{
		result += std::to_string(head);
	}
	else
	{
		// normally I would throw an exception here
	}
	
	result += StringConcat(tail...);
	return result;
}

int main()
{
	// *** Tasks a to g
	std::cout << "---------------" << std::endl;
	std::vector<std::unique_ptr<Job>> jobs;
	jobs.push_back(std::make_unique<Programmer>());
	jobs.push_back(std::make_unique<Pilot>());

	for (const auto& job : jobs)
	{
		std::cout << "* Job" << std::endl;
		std::cout << job.get()->GetJobName() << ": " << job.get()->GetJobDescription() << std::endl;
		std::cout << "Required Hours: " << job.get()->GetJobRequiredHours() << std::endl;
		std::cout << std::endl;
		job.get()->DoWork();

		std::cout << std::endl;
	}

	// *** Task h
	std::cout << "---------------" << std::endl;
	IsInBounds(2, 1, 10);
	IsInBounds(0, 1, 10);
	IsInBounds('b', 'a', 'c');
	IsInBounds('a', 'b', 'c');
	
	std::uint32_t httpResponseCode = 500;
	IsInBounds(httpResponseCode, static_cast<std::uint32_t>(500), static_cast<std::uint32_t>(599));
	httpResponseCode = 401;
	IsInBounds(httpResponseCode, static_cast<std::uint32_t>(500), static_cast<std::uint32_t>(599));

	/* note: the line below wont compiling since the function checks if given value are printable and
	 * supports operators >= and <=.
	 */
	// IsInBounds(Job("a", "a", 40), Job("a", "a", 40), Job("a", "a", 40));

	// *** Task i
	std::cout << "---------------" << std::endl;
	auto comparisonLambda = [](const std::string tested) {
		return (tested == "test");
	};

	std::vector<std::string> theStrings = { "one", "two", "test" };
	auto count = ContainsTheString(comparisonLambda, theStrings);
	std::cout << "Sample1: There are [" << count << "] word(s) \"test\" in the provided sample" << std::endl;

	theStrings.pop_back();
	count = ContainsTheString(comparisonLambda, theStrings);
	std::cout << "Sample2: There are [" << count << "] word(s) \"test\" in the provided sample" << std::endl;

	theStrings.push_back("test");
	theStrings.push_back("test");
	count = ContainsTheString(comparisonLambda, theStrings);
	std::cout << "Sample3: There are [" << count << "] word(s) \"test\" in the provided sample" << std::endl;

	// *** Task j
	std::cout << std::endl << "---------------" << std::endl;
	
	std::string begin = "I have ";
	auto resultedString = StringConcat(begin, 2, " beautiful dogs. Their names starts with ", 'J', " and ", 'Z');
	std::cout << "Result of StringConcat: " << resultedString << std::endl;

	return 0;
}
