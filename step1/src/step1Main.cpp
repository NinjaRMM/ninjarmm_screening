#include <vector>
#include <memory>

#include "Job.h"
#include "Pilot.h"
#include "Programmer.h"
#include "DemoCode.h"

using namespace std;
using namespace ninjans;


/**************************************************************************
 * Template function IsInBounds
 * 	Checks whether value 'v' is in between 'low' and 'high'
 **************************************************************************/
template<class V, class R>
bool IsInBounds(const V &v, const R &low, const R &high)
{
	cout << "IsInBounds value: " << v << " low: " << low << " high: " << high <<endl;
	return v >= low && v <= high;
}


int main(int argc, char **argv)
{
	std::vector<std::unique_ptr<Job>> jobs;

	cout << "\n=========================================================================" << endl;
	cout << "==Question a..j dynamically create object on vector" << endl;
	cout << "=========================================================================" << endl;

	int jobPairCount = 2;

	cout << "Creating " << jobPairCount << " pair of jobs" <<endl;

	//Utilizing unique_ptr to reclaim memory from dynamically allocated objects.

	while(jobPairCount >0)
	{
		std::unique_ptr<Job> programmer(new Programmer());
		jobs.push_back(std::move(programmer));

		std::unique_ptr<Job> pilot(new Pilot());
		jobs.push_back(std::move(pilot));

		jobPairCount--;

	}

	for (auto &el : jobs)
	{
		// Objects polymorphically prints data.
		cout << el->Title() << ":" << endl;
		cout << "\tHasUniform: " << el->HasUniform() << endl;
		cout << "\tIsRemoteWorker: " << el->IsRemoteWorker() << endl;
		cout << "\tWork: " << el->Work() << endl;
	}

	cout << "\n=========================================================================" << endl;
	cout << "==== Question h: Template function IsInBounds test ==" << endl;
	cout << "=========================================================================" << endl;

	bool bInb = IsInBounds<int>(15, 1, 10);
	cout << "numeric test result: " << (bInb ? "true" : "false") << endl << endl;

	bInb = IsInBounds<std::string>("c", "a", "d");
	cout << "string test result: " << (bInb ? "true" : "false") << endl << endl;



	cout << "\n=========================================================================" << endl;
	cout << "==== Question i: test function on array " << endl;
	cout << "=========================================================================" << endl;
	std::vector<std::string> theStrings { "one", "two", "three", "test", "test" };
	std::string testString = "test";

	// Lambda test function captures local variable 'testString'
	auto testFunction = [&testString](std::string data) { return (testString == data);};

	// Lambda to count occurrence.
	auto countFunction = [&testFunction](std::vector<std::string> &arr, decltype (testFunction) &f) {
		int count =0;
		for(auto &el: arr)
		{
			if(f(el))
				count++;
		}

		return count;
	};

	int totalCount = countFunction(theStrings, testFunction);

	cout << "Input vector: [";

	for (auto &el : theStrings)
		cout << el << ",";
	cout << "]" << endl;

	cout << "test String: " << testString << endl;
	cout << "totalCount: " << totalCount << endl;

	cout << "\n=========================================================================" << endl;
	cout << "==== Custom demo automatic cleanup of variables allocated via malloc " << endl;
	cout << "=========================================================================" << endl;
	demoScopedPointerCustom();


	cout << "\n=========================================================================" << endl;
	cout << "==== Custom demo Variadic templates. " << endl;
	cout << "==== Variadic templates allow variable number of template arguments to be " << endl;
	cout << "==== provided to a function." << endl;
	cout << "=========================================================================" << endl;
	cout << "expects 12: result: " << addValues(1,2,4,5) << endl;
	cout << "expects 21: result: " << addValues(10,11) << endl;
	cout << "expects ABC + DEF = ABCDEF: result: " << addValues<string>("ABC","DEF") << endl;
}



