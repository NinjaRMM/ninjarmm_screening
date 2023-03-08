#include <iostream>
#include <vector>
#include <functional>
#include <cassert>

using namespace std;

//a.
class Job {
public:
	virtual string getName() = 0;
	virtual string getDescription() = 0;
	virtual int getRequiredHours() = 0;
	void doWork() {
		cout << "My work involves " + getDescription() << endl;
	}

};

//b.
class Programmer: public Job {
public:
	string getName() override {
		return "Programmer";
	}
	string getDescription() override {
		return "writing software";
	}
	int getRequiredHours() override {
		return 10000;
	}
};




//c.
class Pilot: public Job {
public:
	string getName() override {
		return "Pilot";
	}
	string getDescription() override {
		return "plane flying";
	}
	int getRequiredHours() override {
		return 5;
	}
};


//h.
template<typename T>
bool isInBounds(T value, T minimum, T maximum) {
    return (value >= minimum && value <= maximum);
}


//i.
int containsTheString(function<bool(const string&)> testFunc, const vector<string>& strings) {
    int count = 0;
    for (const auto& str : strings) {
        if (testFunc(str)) {
            count++;
        }
    }
    return count;
}


//j.
template<typename T>
void printArgs(T arg) {
    cout << arg << std::endl;
}

template<typename T, typename... Args>
void printArgs(T arg, Args... args) {
    cout << arg << ", ";
    printArgs(args...);
}

//k.
void testProgrammer() {
	Programmer programmer;
    assert(programmer.getName() == string("Programmer"));
    assert(programmer.getDescription() == string("writing software"));
    assert(programmer.getRequiredHours() == 10000);
}

void testPilot() {
    Pilot pilot;
    assert(pilot.getName() == string("Pilot"));
    assert(pilot.getDescription() == string("plane flying"));
    assert(pilot.getRequiredHours() == 5);
}

void testIsInBounds() {
    assert(isInBounds<int>(5, 0, 10));
    assert(!isInBounds<int>(-5, 0, 10));
    assert(!isInBounds<double>(3.14, 0.0, 3.0));
}

void testContainsTheString() {
	vector<string> strings = {"one", "two", "three", "four"};
    auto count = containsTheString([](const std::string& str) { return str == "two"; }, strings);
    assert(count == 1);

    count = containsTheString([](const std::string& str) { return str.length() == 3; }, strings);
    assert(count == 2);

    count = containsTheString([](const std::string& str) { return str.length() > 4; }, strings);
    assert(count == 1);
}

void testAll() {
	cout << "Running tests..." << endl;
	testProgrammer();
	testPilot();
	testIsInBounds();
	testContainsTheString();
	cout << "All tests passed!" << endl;
}


int main(int argc, char **argv) {
	//e.
	Programmer *programmer = new Programmer();
	Pilot *pilot = new Pilot();
	//f.
	vector<Job *> jobVec{programmer, pilot};

	//g.
	for(auto jobPntr: jobVec) {
		jobPntr->getName();
		jobPntr->getDescription();
		jobPntr->getRequiredHours();
		jobPntr->doWork();
	}
	cout << endl;

	delete programmer;
	delete pilot;

	//h.
	uint32_t httpResonseCode {400};
	auto resp = isInBounds<uint32_t>(httpResonseCode, 500, 599);
	cout << "isInBounds:\n  value:" << httpResonseCode << endl;
	cout << "  minimum: 500" << endl;
	cout << "  maximum: 599" << endl;
	cout << "  result: " << resp <<endl;
	cout << endl;

	//i.
    vector<string> theStrings = {"one", "two", "test"};
    int count = containsTheString([](const string& tested) { return tested == "test"; }, theStrings);
    cout << "containsTheString result: " << count << endl;
	cout << endl;

    //j.
    printArgs(1337);
    printArgs("hello", 0.743, 0xAF45, 0b0110, isInBounds<uint32_t>(httpResonseCode, 500, 599));
    cout << endl;

    //k.
    testAll();

	return 0;
}
