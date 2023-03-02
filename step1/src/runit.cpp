
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
using namespace std;

#include "job.hpp"
#include "pilot.hpp"
#include "programmer.hpp"

#define UNIT_TESTING 1

template<class T> 
bool IsInBounds(T responseCode, T lowerBound, T upperBound) {
	bool result;

	result = (responseCode >= lowerBound) && (responseCode <= upperBound);

	return (result);
};

uint32_t ContainsTheString(bool (*function)(const std::string& stringToLookFor), vector<std::string> stringsToLookAt) {
	uint32_t count = 0;
	for (auto thisString : stringsToLookAt) {
		if (function(thisString)) {
			count++;
		}
	}

	return count;
}



int main() {

	// Spawn one type of each job.
	Pilot *herjob = new Pilot;
	Programmer *myjob = new Programmer;

	// Create a collection.
	vector<Job*> jobs;
	jobs.push_back((Job*) herjob);
	jobs.push_back((Job*) myjob);

	// Hold the values returned in order to print them out.
	string name, desc;
	int hours;
	
	for (auto &thisjob : jobs) {
		name = thisjob->jobname();
		desc = thisjob->jobdesc();
		hours = thisjob->jobhours();
		cout << "The " << name << " job involves " << desc << " and will take " << hours << " hours.\n";
		thisjob->doWork();

		if (UNIT_TESTING == 1) {
			if (name.compare("pilot") == 0) {
				assert(desc.compare("flying an airplane") == 0);
				assert(hours == 56);
			} else if (name.compare("programmer") == 0) {
				assert(desc.compare("solving puzzles") == 0);
				assert(hours == 40);
			}
		}
	}

	/* TDD would require writing tests first and then writing code that passed those tests.
	 * I strongly regret not doing that, but I'm obligated to submit this today. 
	 */



	// Release the memory.
	for (auto&thisjob : jobs) {
		delete thisjob;
		thisjob = 0;
	}

	// Next, take an http response code and test which range it is in.

	vector<uint32_t> inputCodes = { 200, 403, 404, 502, 503, 666, 101 };
	vector<uint32_t> validCodes = { 200, 200 };
	vector<uint32_t> notFoundCodes= { 400, 499 };
	vector<uint32_t> httpErrorCodes = { 500, 599 };

	for (auto &thisCode : inputCodes) {
		cout << "The code " << thisCode;
		if (IsInBounds(thisCode, validCodes[0], validCodes[1])) {
			cout << " is a valid code, within the range of " << validCodes[0] << " through " << validCodes[1] << ".\n";
		} else if (IsInBounds(thisCode, notFoundCodes[0], notFoundCodes[1])) {
			cout << " was an invalid page code, within the range of " << notFoundCodes[0] << " through " << notFoundCodes[1] << ".\n";
		} else if (IsInBounds(thisCode, httpErrorCodes[0], httpErrorCodes[1])) {
			cout << " reflects a server error, within the range of " << httpErrorCodes[0] << " through " << httpErrorCodes[1] << ".\n";
		} else {
			cout << " is unexpected.\n";
		}
	}	
	
	auto theStrings = vector<std::string> { "one", "two",  "test"};

	auto count = ContainsTheString([](const std::string& tested) { return tested == "test" ; }, theStrings);
	cout << "The string appears in the set " << count << " times\n";

}