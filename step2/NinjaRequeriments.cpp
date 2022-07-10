//////////////////////////////////////////
//
//  NinjaOne Requeriments code
//
//  Author : 
//		Arturo Guerra Resendiz
//  Date:  
//		10 / Jul / 2020
//
//	Compiler:
//		Visual Studio
//		
//	Special Rqueriments:
//		Use thread library
//
/////////////////////////////////////////

#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <sstream>
#include <windows.h>

#define __STDCPP_THREADS__ 



//////////////////////////////////////////
//
//  Code for a) to g) points
//
/////////////////////////////////////////

class Job
	{
	private:

		std::string EmployeName;
		std::string Error;

	public :

		Job(std::string employeName);
		virtual ~Job();
		std::string virtual GetName(void){ return Error; };
		std::string virtual GetDescription(void){ return Error; };
		int virtual GetNumberOfHours(){ return 0; };
		void DoWork(void);
	};

class Programmer : public Job
	{
	private:

		std::string Name;
		std::string Description;
		int NumberOfHours;

	public:

		Programmer(std::string employeName);
		~Programmer();
		std::string GetName(void);
		std::string GetDescription(void);
		int GetNumberOfHours();

	};

class Pilot : public Job
	{
	private:

		std::string Name;
		std::string Description;
		int NumberOfHours;

	public:

		Pilot(std::string employeName);
		~Pilot();
		std::string GetName(void);
		std::string GetDescription(void);
		int GetNumberOfHours();
	};

Job::Job(std::string employeName)
	{
	Error = "Error";
	EmployeName = employeName;
	}

Job::~Job()
	{
	}

void Job::DoWork(void)
	{
	std::cout << "My work involves " << GetDescription() << std::endl;
	}

Programmer::Programmer(std::string employeName) : Job(employeName)
	{
	Name = "Programmer";
	Description = "Develop programs";
	NumberOfHours = 40;
	}

Programmer::~Programmer()
	{
	}

std::string Programmer::GetName(void)
	{
	return Name;
	}

std::string Programmer::GetDescription(void)
	{
	return Description;
	}

int Programmer::GetNumberOfHours()
	{
	return NumberOfHours;
	}


Pilot::Pilot(std::string employeName) : Job(employeName)
	{
	Name = "Pilot";
	Description = "Flight planes";
	NumberOfHours = 20;
	}

Pilot::~Pilot()
	{
	}

std::string Pilot::GetName(void)
	{
	return Name;
	}

std::string Pilot::GetDescription(void)
	{
	return Description;
	}

int Pilot::GetNumberOfHours()
	{
	return NumberOfHours;
	}

//////////////////////////////////////////
//
//  Code for h) point
//
/////////////////////////////////////////

template <typename T>
bool IsInBounds(T httpCode, uint32_t Min, uint32_t Max)
	{
	bool localResult = false;

	if( (httpCode >= Min) && (httpCode <= Max))
		localResult = true;

	return localResult;
	}

//////////////////////////////////////////
//
//  Code for i) point
//
/////////////////////////////////////////

int ContainsTheString(std::string &tested, std::vector<std::string> theStrings)
	{
	int count = 0;
	std::vector<std::string>::iterator it;

	for (it = theStrings.begin(); it != theStrings.end(); ++it)
		{
		if( *it == tested)
			{
			++count;
			}
		}

	return count;
	}

void FunctionCount(void)
	{
	const char *args[] = { "one", "two",  "test"};
	std::vector<std::string> theStrings(args, std::end(args));
	int count = 0;
	std::string test = "test";

	count = ContainsTheString(test, theStrings);

	std::cout << count << std::endl;
	}

//////////////////////////////////////////
//
//  Code for j)  point
//
// The propouse of this code is load the Pilot and Programmer objects 
// in asynchonous way using two different threads
//
/////////////////////////////////////////


class JobLoad
	{
	private:

		std::vector<Job *> JobCollection;
		HANDLE Semaphore;
		bool PilotLoadFinish;
		bool ProgrammerLoadFinish;

	public:
		JobLoad(void);
		~JobLoad();
		void JobLoadThreadPilot(void);
		void JobLoadThreadProgrammer(void);
	};

void ThreadJobLoadPilot( JobLoad *JobLoadPtr);
void ThreadJobLoadProgrammer( JobLoad *JobLoadPtr);

JobLoad::JobLoad(void)
	{
	bool exitFlag = false;

	Semaphore = CreateSemaphore( NULL, 1, 1, NULL);
	ProgrammerLoadFinish = false;
	PilotLoadFinish = false;
	std::thread loadPilot(ThreadJobLoadPilot, this);
	std::thread loadProgrammer(ThreadJobLoadProgrammer, this);
	loadPilot.detach();
	loadProgrammer.detach();

	while( exitFlag == false)
		{
		WaitForSingleObject(Semaphore, INFINITE);
		if( PilotLoadFinish == true && ProgrammerLoadFinish == true )
			exitFlag = true;
		else
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		ReleaseSemaphore(Semaphore, 1, NULL);
		}

	std::cout << "Number of employees is : " << JobCollection.size() << std::endl;

	}

JobLoad::~JobLoad()
	{
	CloseHandle(Semaphore);
	}

void JobLoad::JobLoadThreadPilot(void)
	{
	int localNumOfJobsIndex = 0;
	std::string JobId;
	Pilot *pilot;

	while( localNumOfJobsIndex < 20 )
		{
		JobId.clear();
		JobId.assign("Pilot_");
		std::stringstream jobNumber;
		jobNumber << localNumOfJobsIndex;
		JobId.append(jobNumber.str());
		WaitForSingleObject(Semaphore, INFINITE);
		pilot = new Pilot(JobId);
		JobCollection.push_back(pilot);
		ReleaseSemaphore(Semaphore, 1, NULL);
		++localNumOfJobsIndex;
		}

	WaitForSingleObject(Semaphore, INFINITE);
	PilotLoadFinish = true;
	ReleaseSemaphore(Semaphore, 1, NULL);
	}

void JobLoad::JobLoadThreadProgrammer(void)
	{
	int localNumOfJobsIndex = 0;
	std::string JobId;
	Programmer *programmer;

	while( localNumOfJobsIndex < 20 )
		{
		JobId.clear();
		JobId.assign("Programmer_");
		std::stringstream jobNumber;
		jobNumber << localNumOfJobsIndex;
		JobId.append(jobNumber.str());
		WaitForSingleObject(Semaphore, INFINITE);
		programmer = new Programmer(JobId);
		JobCollection.push_back(programmer);
		ReleaseSemaphore(Semaphore, 1, NULL);
		++localNumOfJobsIndex;
		}

	WaitForSingleObject(Semaphore, INFINITE);
	ProgrammerLoadFinish = true;
	ReleaseSemaphore(Semaphore, 1, NULL);
	}

void ThreadJobLoadPilot(JobLoad *jobLoadPtr)
	{
	if( jobLoadPtr != NULL)
		{
		jobLoadPtr->JobLoadThreadPilot();
		}
	}

void ThreadJobLoadProgrammer(JobLoad *jobLoadPtr)
	{
	if( jobLoadPtr != NULL)
		{
		jobLoadPtr->JobLoadThreadProgrammer();
		}
	}


int main(int argc, char **argv)
	{

	//////////////////////////////////////////
	//
	//  Code for a) to g) points
	//
	/////////////////////////////////////////

	std::vector<Job *> jobCollection;
	std::vector<Job *>::iterator it;
	Job *jobPtr;

	Pilot *pilot1 = new Pilot("Peter");
	Pilot *pilot2 = new Pilot("Carl");
	Programmer *programmer1 = new Programmer("Arthur");
	Programmer *programmer2 = new Programmer("Carl");

	jobCollection.push_back(pilot1);
	jobCollection.push_back(pilot2);
	jobCollection.push_back(programmer1);
	jobCollection.push_back(programmer2);

	for( it = jobCollection.begin(); it != jobCollection.end(); ++it)
		{
		jobPtr = (Job *)*it;
		std::string JobName = jobPtr->GetName();
		std::string JobDescription = jobPtr->GetDescription();
		int JobNumberOfHours = jobPtr->GetNumberOfHours();
		jobPtr->DoWork();
		}

	delete pilot1;
	delete pilot2;
	delete programmer1;
	delete programmer2;

	//////////////////////////////////////////
	//
	//  Code for h)  point
	//
	/////////////////////////////////////////

	uint32_t httpResonseCode = 550;
	uint32_t minValue = 500;
	uint32_t maxValue = 599;
	bool result = IsInBounds<uint32_t>(httpResonseCode, minValue, maxValue);

	std::cout << "Min Value : " << minValue << std::endl;
	std::cout << "Max Value : " << maxValue << std::endl;
	std::cout << "httpResonseCode : " << httpResonseCode << std::endl;
	if( result == true)
		std::cout << "httpResonseCode is into the range" << std::endl;
	else
		std::cout << "httpResonseCode is not into the range" << std::endl;


	//////////////////////////////////////////
	//
	//  Code for i)  point
	//
	/////////////////////////////////////////

	FunctionCount();

	//////////////////////////////////////////
	//
	//  Code for i)  point
	//
	/////////////////////////////////////////

	class JobLoad *jobLoad = new JobLoad();
	delete jobLoad;

	return 0;
	}
