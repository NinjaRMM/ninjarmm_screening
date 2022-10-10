#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

#define PILOT_ROLE "Formulating a flight plan taking into accordance \
aircraft performance, altitude, fuel, and weather and flying the plane"

#define PROGRAMMER_ROLE "Write code for computer programs and mobile \
applications, maintaining, debugging and troubleshooting systems and \
software to ensure that everything is running smoothly."

#define PILOT_ID        0
#define PROGRAMMER_ID   1

class job
{
    static int id_num;
protected:
    string name;
    string description;
    long hoursReq;
public:
    job(void)
    {
        id_num++;
    }
    void DoWork (void)
    {
        cout << endl << "My work involves " << description << endl;
    }
    long getHoursReq (void)
    {
        return hoursReq;
    }
    string getName (void)
    {
        return name;
    }
    string getDescription (void)
    {
        return description;
    }
};
int job::id_num = 0;

class pilot : public job
{
public:
    pilot(void)
    {
        name = string("Pilot");
        description = string(PILOT_ROLE);
        hoursReq = 30000;
    }
};

class programmer : public job
{
public:
    programmer(void)
    {
        name = string("Programmer");
        description = string(PROGRAMMER_ROLE);
        hoursReq = 2000;
    }
};


/*INPUT: 13 1 0 0 1 0 1 1 1 0 0 1 1 1*/

int main (int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    int input_len = 0;
    int select_role = 0;
    vector<job*> job_vec;

    cin >> input_len;

    for (int i = 0; i < input_len; i++)
    {
        cin >> select_role;

        if(select_role == PILOT_ID) {
            pilot* role_pilot = new pilot();  
            job_vec.push_back(role_pilot);
        } else if (select_role == PROGRAMMER_ID) {
            programmer* role_prog = new programmer();
            job_vec.push_back(role_prog);
        } else {
            cout << "Fail to receive ID role" << endl;
        }        
    }
   
    for (int i = 0; i < input_len; i++)
    {
        cout << endl << job_vec[i]->getName() << endl; 
        cout << endl << job_vec[i]->getDescription() << endl;
        cout << endl << job_vec[i]->getHoursReq() << endl;
        job_vec[i]->DoWork();
    }

    //I stopped development here, I'm out of time to continue
    //I will try get back of this tomorrow
   
   return 0;
}
