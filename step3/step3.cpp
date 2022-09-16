//file header and copyright here
//*******************************************************************
//includes
//#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
//*******************************************************************
class Job
{
public: //typedefs
	typedef std::vector<Job *> TVector;

public://methods
	Job(const std::string &name,const std::string &descr,int hours)
	:m_Name(name),
	 m_Description(descr),
	 m_HoursRequired(hours)
	{
	}

	virtual ~Job()
	{
	}

	std::string GetName() const					{return m_Name;}
	void SetName(const std::string &s)			{m_Name = s;}

	std::string GetDescription() const			{return m_Description;}
	void SetDescription(const std::string &s)	{m_Description = s;}

	//derived overrides
	virtual std::string GetInfo() const			{return "unknown";}

	int GetHoursRequired() const				{return m_HoursRequired;}
	void SetHoursRequired(int hr)				{m_HoursRequired = hr;}

	virtual void DoWork()
	{
		std::cout << "My work involves " << m_Description << std::endl;
	}

protected://data
	std::string m_Name;
	std::string m_Description;
	int m_HoursRequired;
};
//*******************************************************************
class Programmer : public Job
{
public://methods
	Programmer(const std::string &name,int hours_required = 100)
	:Job(name,"programming",hours_required)
	{
	}

	virtual ~Programmer()
	{
	}

	virtual std::string GetInfo() const			{return "C++, Qt, Win";}

	virtual void DoWork()
	{
		Job::DoWork();
		//do Programmer specific stuff here
	}
};
//*******************************************************************
class Pilot : public Job
{
public://methods
	Pilot(const std::string &name,int hours_required = 32)
	:Job(name,"flying planes",hours_required)
	{
	}

	virtual ~Pilot()
	{
	}

	virtual std::string GetInfo() const			{return "Cessna single prop";}

	virtual void DoWork()
	{
		Job::DoWork();
		//do Pilot specific stuff here
	}
};
//*******************************************************************
template <typename T = int>
inline bool IsInBounds(T val,const T first, const T last)
{
	if (val < first)
		{return false;}

	if (val > last)
		{return false;}

	return true;
}
//*******************************************************************
bool IsMatch(const std::string &s)
{//note: not using a lambda
	if (s == "test")
		{return true;}

	return false;
}
typedef bool (*TTestFunc)(const std::string &s);
//*******************************************************************
typedef std::vector<std::string> TStringList;
int ContainsTheString(const TStringList &strs,TTestFunc pf)
{
	int count = 0;
	for (TStringList::const_iterator it = strs.begin();
		 it != strs.end();
		 ++it)
	{
		const std::string &s = *it;
		if (pf(s))
			{++count;}
	}
	return count;
}
//*******************************************************************
//bonus variadic template stuff
void PrintArg()
{
	std::cout << std::endl;
}
//*******************************************************************
template<typename T>
void PrintArg(const T &t)
{//special case for one param
	std::cout << t << std::endl;
}
//*******************************************************************
template<typename First, typename... Rest>
void PrintArg(const First &first, const Rest&... rest)
{//at least 2 arg's because overloads above catch 0 and 1 params
	std::cout << first << ", ";
	PrintArg(rest...);//recursive
}
//*******************************************************************
int main()
{
	Programmer *pprogrammer = new Programmer("Mike");

	Pilot *ppilot = new Pilot("Joe");

	Job::TVector jobs;

	jobs.push_back(pprogrammer);
	jobs.push_back(ppilot);

	for (Job::TVector::iterator it = jobs.begin();
		 it != jobs.end();
		 ++it)
	{
		Job *p = *it;	//polymorphic pointer
		if (!p)
			{continue;}

		std::string name = p->GetName();
		std::string descr = p->GetDescription();
		std::string info = p->GetInfo();
		int hours_req = p->GetHoursRequired();
		std::cout	<< "Name: " << name 
					<< ", Descr: " << descr 
					<< ", Info: " << info
					<< ", Hours: " << hours_req 
					<< std::endl;
		p->DoWork();

		//programmer should get flagged for too many hours
		bool hours_ok = IsInBounds(hours_req,25,80);//type int deduced from param
		hours_ok = IsInBounds<int>(hours_req,25,80);//type is int

		std::cout << hours_req << " hours ok: " << (hours_ok ? "yes" : "no") << std::endl;
	}

	TStringList strs {"one", "two", "test"};
	auto count = ContainsTheString(strs,IsMatch);
	std::cout << "Count of test matches (1): " << count << std::endl;

	auto strs2 = std::vector<std::string> {"one", "two", "test", "three", "four", "test"};
	auto count2 = ContainsTheString(strs2,IsMatch);
	std::cout << "Count of test matches (2): " << count2 << std::endl;

	//bonus variadic template usage:
	PrintArg();
	PrintArg(1);
	PrintArg(5,10);
	PrintArg(20,30,40);
	PrintArg("foo",50,"bar",1.2);
}
//*******************************************************************