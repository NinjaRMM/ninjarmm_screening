#ifndef JOB_HPP
#define JOB_HPP

#include <string>
#include <vector>
#include <iostream>
/* a */
class Job{
    protected:
        int hours_required;
        std::string name;
        std::string description;

    public:
        Job(int _hours, std::string _name, std::string _description);
        
        virtual ~Job(){};
        
        std::string get_name() ;
        std::string get_description();
        int get_job_hours();
        std::string DoWork();
};
/* b */
class Programmer:public Job
{    
    public:     
        Programmer(int hours=10, std::string name="Programmer", std::string description="Software Developer"):
                    Job(hours,name,description){}
        ~Programmer(){};
};
/* c */
class Pilot:public Job
{
    public:
        Pilot(int hours=20, std::string name="Pilot", std::string description="Airplane Pilot"):
                Job(hours,name,description){}
        ~Pilot(){};
};

class FactoryJob
{
    public:
        typedef enum _Job_types{
            PROGRAMMER,
            PILOT
        }Job_types;

     static std::shared_ptr<Job> create(Job_types type, int hours, std::string name, std::string description);
};



#endif
