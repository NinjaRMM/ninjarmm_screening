#include <cstdio>
#include <iostream>
#include <algorithm>


#include <list>
#include <vector>

using std::cout;
using std::endl;

using std::string; 
using std::list;
using std::vector;

class Job{
	string name;
	string description;
	int hours;

public:
	string getName() { return this->name; }
	void setName(const string name) { this->name = name; }

	string getDescription() { return this->description; }
	void setDescription(const string description){ this->description = description; }

	int getHours() { return this->hours; }
	void setHours(int hours) { this->hours = hours; }

	void DoWork(){
		cout << "My work involves " + this->description << endl;
	}
};

class Programmer : public Job {
	public:
	Programmer(){
		setDescription("programming softwares");
		setHours(40);
	}
};

class Pilot : public Job {
	public:
	Pilot(){
		setDescription("piloting");
		setHours(40);
	}
};

template<typename T>
bool IsInBounds(T httpResponseCode, T begin, T end){
	return httpResponseCode >= begin && httpResponseCode <= end;
}

template<typename F>
int ContainsTheString(F fn, vector<std::string> strs){
	int count = 0;
	for( auto str:strs ){
		count += (*fn)(str);
	}
	return count;
}

int main ( int argc, char ** argv ){

	list<Job*> jobs;

	Programmer * programmer = new Programmer();
	Pilot * pilot = new Pilot();
	
	jobs.push_back(programmer);
	jobs.push_back(pilot);


	for ( auto& job : jobs ){
		job->DoWork();
	}

	
	delete programmer;
	delete pilot;

	// h. Create a template function that will test if the provided values are within a range. 
	// Call the function and print out the inputs and result. The following should execute 
	// assuming httpResonse is a uint32_t value:
	
	uint32_t httpResonseCode = 522;
	if ( IsInBounds<uint32_t>(httpResonseCode, 500, 599) ) {
		cout << "In Bounds!" << endl;
	} else { 
		cout << "Out of Bounds!" << endl;
	}

	vector<std::string> theStrings =  { "one", "two",  "test", "test", "test", "test"};


	auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
	cout << "Count is : " << count << endl;

	return 0;
}
