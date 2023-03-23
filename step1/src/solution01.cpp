#include <iostream>
#include <string>

using namespace std;

class Job {
public:
  virtual string GetName() = 0; 
  virtual string GetDescription() = 0;
  virtual unsigned int GetHoursRequired() = 0;
  virtual void DoWork() = 0;
};

class Programmer : Job {
public:
  string GetName() {
    return "Programmer";
  }

  string GetDescription() {
    return "writing computer programs.";
  }

  unsigned int GetHoursRequired() {
    return 8;
  }

  void DoWork() {
    cout << "My work involves " + GetDescription() << endl;
  }
};

class Pilot : Job { 
public:
  string GetName() {
    return "Pilot";
  }

  string GetDescription() {
    return "flying an airplane.";
  }

  unsigned int GetHoursRequired() {
    return 11;
  }

  void DoWork() {
    cout << "My work involves " + GetDescription() << endl;
  }
};


//////////////////////////////////////////////////////////////////////
// Program Entry Point
//////////////////////////////////////////////////////////////////////
int main() {

  Programmer *programmer = new Programmer();
  Pilot *pilot = new Pilot();

  programmer->DoWork();
  pilot->DoWork();

  return 0;
}

