#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Job {
    public:
      string GetName() {return m_name;}
      string GetDescription() {return m_description;}
      int GetHoursRequired() {return m_hoursRequired;}
      
      void DoWork() {
          cout << DO_WORK_TEMPLATE << m_description << endl;
      }
      
    protected:
      Job(string name, string description, int hoursRequired)
        :  m_name(name), m_description(description), m_hoursRequired(hoursRequired)
        {
        }
      
    private:
      const string DO_WORK_TEMPLATE = "My work involves ";
      string m_name;
      string m_description;
      int m_hoursRequired;
};

class Programmer: public Job {
    public:
      Programmer(string description, int hoursRequired):Job("Programmer", description, hoursRequired)
      {
      }
    
};

class Pilot: public Job {
    public:
      Pilot(string description, int hoursRequired):Job("Pilot", description, hoursRequired)
      {
      }

};

template <typename myType>
myType IsInBounds (myType value, myType lowRange, myType highRange) {
    return ((value > lowRange) && (value < highRange));
}

bool tested (string compare) {
    return (compare == "test");
}

int ContainsTheString(bool (*function) (string), vector<string> theStrings) {
    int count = 0;
    for (int i = 0; i < theStrings.size(); i++) {
      if ((*function)(theStrings[i])) {
        count++;
      }
    }
    return count;
}

int main()
{
  vector<Job*> jobs;
  jobs.push_back(new Programmer("Developing code", 40));
  jobs.push_back(new Pilot("Flying airplanes", 52));
  const int LOW_RANGE = 32;
  const int HIGH_RANGE = 50;
  auto theStrings = vector<string> { "one", "two",  "test"};
  auto count = ContainsTheString(tested, theStrings);
  
  cout << "Count: " << count << endl;
  
  for (int i = 0; i < jobs.size(); i++) {
    cout << "Name: " << jobs[i]->GetName() << endl;
    cout << "Description: " << jobs[i]->GetDescription() << endl;
    cout << "Hours required: " << jobs[i]->GetHoursRequired() << endl;
    cout << "Lower range: " << LOW_RANGE << endl;
    cout << "Higher range: " << HIGH_RANGE << endl;
    if (IsInBounds<int>(jobs[i]->GetHoursRequired(), LOW_RANGE, HIGH_RANGE)) {
        cout << "Result: hours required is within range" << endl;
    }
    else {
        cout << "Result: hours required is not within range" << endl;
    }
    jobs[i]->DoWork();
  }
  
  return 0;
}
