#include "job.hpp"
#include <algorithm>

/* a */
Job::Job( int _hours, 
          std::string _name,
          std::string _description):
          hours_required(_hours),
          name(_name),
          description(_description)
{    
}

/* da */
std::string Job::get_name(){
    return this->name;
}
std::string Job::get_description(){
    return this->description;
}
int Job::get_job_hours(){
    return this->hours_required;
}
std::string Job::DoWork(){
    std::string work= "My work involves " + this->description;
    return work;
}

std::shared_ptr<Job> FactoryJob::create(Job_types type, int hours, std::string name, std::string description){
    switch (type)
    {
        
        case PROGRAMMER:
            return std::shared_ptr<Programmer>(new Programmer(hours,name,description));
        break;

        case PILOT:
            return std::shared_ptr<Pilot>(new Pilot(hours,name,description));
        break;      
    
        default:
            return std::shared_ptr<Job>(new Job(hours,name,description));
        break;     
    }
}
/* e,f,g */
void allocate_jobs()
{
    std::vector<std::shared_ptr<Job> > jobs;    
    FactoryJob creator;

    jobs.push_back(creator.create(FactoryJob::PROGRAMMER, 10,"Programmer", "Senior Software Developer"));
    jobs.push_back(creator.create(FactoryJob::PILOT, 11,"Pilot", "Boeing Pilot"));

    for( auto it: jobs){
        printf("Name: %s\nDescription: %s\nHours Required: %d\nDoWork: %s\n\n",
                it->get_name().c_str(),
                it->get_description().c_str(),
                it->get_job_hours(),
                it->DoWork().c_str() );

    }
}

/* h */
template<typename T>
void IsInBounds(T code, T min, T max)
{
    if( min <= code) {
        std::cout <<"The input value ["<< code << "] is lower than mininum range. Range ["<< min << "]["<< max<<"]" << std::endl;
    } else if(  code >= max) {
        std::cout <<"The input value ["<< code << "] is higher than maximum range. Range ["<< min << "]["<< max<<"]" << std::endl;
    } else{
        std::cout << "The input value ["<< code << "] is within range ["<< min << "]["<< max<<"]" << std::endl;
    }    
}

bool ContainsTheString( const std::string &input) {
    if( input == "test"){
        return true;
    }else{
        return false;
    }
}

int count_match( std::vector<std::string> &theStrings , bool (*func)(const std::string&) )
{
    
    auto count= std::count_if(theStrings.begin(), theStrings.end(), [func](std::string &s) {  return (*func)(s); }  );

    return count;
}
void test_count()
{
    auto theStrings = std::vector<std::string> { "one", "two", "test", "test"}; 
    
    std::cout << " count " << count_match( theStrings, ContainsTheString) << std::endl;
}

int main(){
    allocate_jobs();
    IsInBounds<int>(1,2,3);
    IsInBounds<char>('e','a','b');
    test_count();
    return 0;
}