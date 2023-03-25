#include <vector>
#include <memory>
#include <cstdio>
#include <string>
#include <type_traits>
#include <cassert>


class Job
{
public:
	virtual	~Job() {}

	virtual const char* const GetName() = 0;
	virtual const char* const GetDescription() = 0;
	virtual unsigned int GetHoursRequired() = 0;
	inline void DoWork()
	{
		// Usually, calling a purely virtual function on the base class can be dangerous
		// if you call it in the constructor, but since this is an interface, this
		// call will do a look-up in the vtable for the correct (defined) method.
		printf("My work involves %s.\n", GetDescription());
	}
};


// Few notes about the derived classes:
// * One detail about the way I implemented these classes is that I'm only creating "getters", since
//   that's what was requested for the interface
// * I thought about creating a constructor that would allow me to pass the actual variables representing
//   the name, the description, and the hours required, so that way each class instance would have
//   different values, but since that wasn't explicitly requested, I chose not to
// * I tend to break these down in a header and cpp file, as it's easier to digest the code in separate parts,
//   (except for inline methods), but once again, I figured I could make this fit into one file without
//   bloating it too much

class Programmer : public Job
{
public:
	// Usually, I tend to create a header and a cpp file to separate this, or at the very least I'll do line breaks,
	// but when it's about an empty body, I just put the curly-braces in front of the declaration (less scrolling).
	Programmer() {};
	~Programmer() {};

	const char* const GetName() override
	{
		return "Programmer";
	}

	const char* const GetDescription() override
	{
		return "writing computer programs";
	}

	unsigned int GetHoursRequired() override
	{
		return 8;
	}
};


class Pilot : public Job
{
public:
	Pilot() {}
	~Pilot() {}

	const char* const GetName() override
	{
		return "Pilot";
	}

	const char* const GetDescription() override
	{
		return "operating the flying controls of an aircraft";
	}

	unsigned int GetHoursRequired() override
	{
		return 12;
	}
};


template<typename T>
bool IsInBounds(const T valueToCheck, T min, T max)
{
	return min <= valueToCheck && valueToCheck <= max;
}


template<typename Fn>
int ContainsTheString(Fn&& testFunction, const std::vector<std::string>& theStrings)
{
	static_assert(std::is_invocable_r_v<bool, Fn, const std::string&>, "Method passed should take a const std::string& and return bool!");
	int count = 0;
	for (const auto& str : theStrings)
	{
		if (testFunction(str))
		{
			++count;
		}
	}

	return count;
}


// =========================
// My code (as per request)
// I have my own custom engine, which is an eternal work-in-progress, and I've got a feature that I really like and want to show (it's something I've coded quite some
// time ago, so it may not be all 100%, though it should be). In order to implement my fiber-based job system, I've designed a delegate class, which I can create
// using function pointers or lambdas, the latter being pretty useful since I can have a capture-list. The only issue with the Delegate class is that when
// defining it I cannot use template deduction, so for that I have another class, Task, which just wraps some data array) - it is somewhat hacky stuff,
// but it gets the job done in a nice way

// Base delegate class
class IDelegate
{
public:
	virtual ~IDelegate() {}
	virtual void Invoke() const = 0;
};


// Delegate class, which uses C++ 17's std::apply (in place of index sequences) to call a function with variadic template arguments
template<class Fn, class... Args>
class Delegate final : public IDelegate
{
public:
	// I'm making sure the return type is void and it's a callable function
	template<typename = typename std::enable_if_t<std::is_same_v<std::invoke_result_t<Fn, Args...>, void> && std::is_invocable_v<Fn, Args...>>>
	Delegate(Fn && fn, Args &&... args)
		: m_Function(std::forward<Fn>(fn)), m_Args(std::forward<Args>(args)...) {}
	~Delegate() {}

	void Invoke() const override
	{
		std::apply(m_Function, m_Args);
	}

private:
	Fn m_Function;
	std::tuple<Args...> m_Args;
};


// Main class that wraps the functionality of the Delegate classes in order to provide an easier, less-typing,
// and no less efficient solution to perform random tasks
class Task
{
public:
	static constexpr auto kTaskSize = std::hardware_destructive_interference_size; // Cache-line size

	Task() {}

	template<typename Fn, typename... Args,
		typename = typename std::enable_if_t<
		!std::is_same_v<std::decay_t<Fn>, Task> // This is to be sure I'm not passing a Task object on this,
		&& std::is_invocable_v<Fn, Args...> // It's actually a method and you can call it
		&& std::is_same_v<std::invoke_result_t<Fn, Args...>, void>>> // Again, the return type has to be void
	explicit Task(Fn&& fn, Args &&... args)
	{
		static_assert(sizeof(Delegate<Fn, Args...>) <= kTaskSize, "Job object is too big");
		Set(std::forward<Fn>(fn), std::forward<Args>(args)...);
	}

	explicit Task(const Task& rhs)
	{
		memcpy(m_Data, rhs.m_Data, kTaskSize);
	}

	Task& operator=(const Task& rhs)
	{
		if (this != &rhs)
		{
			this->~Task();
			memcpy(m_Data, rhs.m_Data, kTaskSize);
		}

		return *this;
	}

	explicit Task(Task&& rhs) noexcept
	{
		*this = std::move(rhs);
	}

	Task& operator=(Task&& rhs) noexcept
	{
		if (this != &rhs)
		{
			this->~Task();
			memcpy(m_Data, rhs.m_Data, kTaskSize);
			rhs.~Task();
		}

		return *this;
	}
	~Task()
	{
		if (IsSet())
		{
			reinterpret_cast<IDelegate* const>(m_Data)->~IDelegate();
			Reset();
		}
	}

	void Run() const
	{
		// Trick to show text in console
		assert(IsSet() && "Job doesn't have an entry point");
		reinterpret_cast<const IDelegate* const>(m_Data)->Invoke();
	}

	template<typename Fn, typename... Args,
		typename = typename std::enable_if_t<
		!std::is_same_v<std::decay_t<Fn>, Task> // This is to be sure I'm not passing a Task object on this,
		&& std::is_same_v<std::invoke_result_t<Fn, Args...>, void>>> // Again, the return type has to be void
	void Set(Fn&& fn, Args &&... args)
	{
		static_assert(sizeof(Delegate<Fn, Args...>) <= kTaskSize, "Job object is too big");
		new (m_Data) Delegate<Fn, Args...>(std::forward<Fn>(fn), std::forward<Args>(args)...);
	}

	bool IsSet() const
	{
		// Very hacky, but faster to check
		auto vp = *reinterpret_cast<const unsigned long long* const>(m_Data);
		return vp != 0;
	}

	void Reset()
	{
		// Very hacky, but faster to clear
		auto pVp = reinterpret_cast<unsigned long long* const>(m_Data);
		pVp = 0;
	}

private:
	unsigned char m_Data[kTaskSize]{};
};


void TestFunction(float value)
{
	printf("global function -> %f\n", value);
}


int main()
{
	// No type of collection was specified, so I'm going with one that deals with a contiguous block of memory :)
	// I'm also using unique_ptr since it allows me to simply transfer ownership between the objects and it'll
	// handle the cleaning for me (though I'm not a fan of smart pointers)
	std::vector<std::unique_ptr<Job>> jobs;
	jobs.emplace_back(std::make_unique<Programmer>());
	jobs.emplace_back(std::make_unique<Pilot>());

	for (auto&& job : jobs)
	{
		printf("Name: %s\n", job->GetName());
		printf("Description: %s\n", job->GetDescription());
		printf("Hours required: %d\n", job->GetHoursRequired());
		job->DoWork();
		printf("\n");
	}
	printf("\n");

	unsigned int minBound = 500;
	unsigned int maxBound = 599;
	unsigned int httpErrorCode1 = 404;
	unsigned int httpErrorCode2 = 500;
	bool result1 = IsInBounds<unsigned int>(httpErrorCode1, minBound, maxBound) == true; // the <unsigned int> part is redundant in this case (all vars are unsigned int)
	bool result2 = IsInBounds<unsigned int>(httpErrorCode2, minBound, maxBound) == true;
	printf("Testing IsInBounds with error code %d and bounds [%d, %d]: %s\n", httpErrorCode1, minBound, maxBound, result1 ? "true" : "false");
	printf("Testing IsInBounds with error code %d and bounds [%d, %d]: %s\n", httpErrorCode2, minBound, maxBound, result2 ? "true" : "false");
	printf("\n\n");

	auto theStrings = std::vector<std::string>{ "one", "two",  "test" };
	auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
	//count = ContainsTheString([]() { return true; }, theStrings); // Uncomment to test the static_assert on the method
	printf("Calling ContainsTheString() with lambda: %d\n\n\n", count);

	// ==============================================
	// My custom code part - using my Task class to
	// showcase work scheduling
	std::vector<Task> tasks;

	// using lambda ctor
	tasks.emplace_back([](int i, int j)
		{
			printf("lambda 1 -> %d\n", i + j);
		}, 2, 6);

	// using Set()
	auto& tsk = tasks.emplace_back();
	tsk.Set([](const char* pStr)
		{
			printf("lambda 2 -> %s\n", pStr);
		}, "test string");

	// using an existing function object
	int a = 30;
	auto lambda = [a]()
	{
		printf("lambda 3 -> %d\n", a);
	};
	tasks.emplace_back(lambda);

	// using a global function
	tasks.emplace_back(TestFunction, 1.04f);

	// Run all of them
	for (auto& task : tasks)
	{
		task.Run();
	}

	return 0;
}