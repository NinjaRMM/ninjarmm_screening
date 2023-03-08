#include <iostream>
#include <vector>

using namespace std;

class Job {
public:
	virtual string getName() = 0;
	virtual string getDescription() = 0;
	virtual int getRequiredHours() = 0;
	void doWork() {
		cout << "My work involves " + getDescription() << endl;
	}

};

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


class Pilot: public Job {
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


int main(int argc, char **argv) {
	Programmer *programmer = new Programmer();
	Pilot *pilot = new Pilot();
	vector<Job *> jobVec{programmer, pilot};

	for(auto jobPntr: jobVec) {
		jobPntr->getName();
		jobPntr->getDescription();
		jobPntr->getRequiredHours();
		jobPntr->doWork();
	}

	delete programmer;
	delete pilot;
	return 0;
}
